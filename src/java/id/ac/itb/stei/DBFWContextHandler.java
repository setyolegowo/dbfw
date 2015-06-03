/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package id.ac.itb.stei;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.wso2.balana.Balana;
import org.wso2.balana.PDP;
import org.wso2.balana.PDPConfig;
import org.wso2.balana.ParsingException;
import org.wso2.balana.ctx.AbstractResult;
import org.wso2.balana.ctx.Attribute;
import org.wso2.balana.ctx.ResponseCtx;
import org.wso2.balana.ctx.xacml3.Result;
import org.wso2.balana.finder.AttributeFinder;
import org.wso2.balana.finder.AttributeFinderModule;
import org.wso2.balana.finder.impl.FileBasedPolicyFinderModule;
import org.wso2.balana.xacml3.Attributes;
import org.xml.sax.SAXException;

/**
 *
 * @author setyo.legowo
 */
public final class DBFWContextHandler {

    private final static Log log = LogFactory.getLog(DBFWContextHandler.class);
    private final static long DIFF_TIME = 1200000; // 1200 seconds == 20 minutes
    private final URI uri_column = URI.create("urn:oasis:names:tc:xacml:3.0:attribute-category:column");
    private static Balana balana = null;
    private static long last_time = 0;
    private String username;
    private String action;
    private String tableResource;
    private String ipclient;
    private String result;
    public ArrayList<String> attrResource = new ArrayList<>();

    public void setAction(String action) {
        this.action = action;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setTableResource(String tableResource) {
        this.tableResource = tableResource;
    }

    public void setIpclient(String ipclient) {
        this.ipclient = ipclient;
    }
    
    public void addColumn(String name) {
        attrResource.add(name);
    }

    public String getUsername() {
        return username;
    }

    public String getAction() {
        return action;
    }

    public String getTableResource() {
        return tableResource;
    }
    
    public String getResult() {
        if(result.trim().length() > 0)
            return result;
        else
            return null;
    }

    public String getIpclient() {
        return ipclient;
    }
    
    public void clearAttrResource() {
        attrResource.clear();
    }
    
    public DBFWContextHandler() {
        attrResource.clear();
        initBalana();
    }
    
    public DBFWContextHandler(String username) {
        this.username = username;
        attrResource.clear();
        initBalana();
    }
    
    public DBFWContextHandler(String username, String action) {
        this.username = username;
        this.action = action;
        attrResource.clear();
        initBalana();
    }
    
    public DBFWContextHandler(String username, String action, String tableResource) {
        this.username = username;
        this.action = action;
        this.tableResource = tableResource;
        attrResource.clear();
        initBalana();
    }
    
    public void run() {
        if(this.username != null && this.username.trim().length() > 0 && 
                this.action != null && this.action.trim().length() > 0 &&
                this.tableResource != null && this.tableResource.trim().length() > 0){

            String request = createXACMLRequest();
            PDP pdp = getPDPNewInstance();
            String response = pdp.evaluate(request);
            Set<String> resultColumns = new HashSet<>();

            try {
                ResponseCtx responseCtx = ResponseCtx.getInstance(getXacmlResponse(response));
                Set<AbstractResult> results  = responseCtx.getResults();
                for(AbstractResult _result : results){
                    Set<Attributes> attributesSet = ((Result)_result).getAttributes();
                    for(Attributes attributes : attributesSet){
                        if(attributes.getCategory().equals(uri_column)) {
                            for(Attribute attribute : attributes.getAttributes()){
                                resultColumns.add(_result.getDecision() + ":" + attribute.getValue().encode());
                            }
                        }
                    }
                }
            } catch (ParsingException e) {
                log.error(e);
            }
            
            this.result = "" + resultColumns.size();
            for(String row : resultColumns) {
                this.result += " " + row;
            }
            this.result += "\n";
        } else {
            log.warn("User name, action, and table resource can not be empty");
        }
    }
    
    /**
     * Returns a new PDP instance with new XACML policies
     *
     * @return a  PDP instance
     */
    private PDP getPDPNewInstance(){

        PDPConfig pdpConfig;
        synchronized(this) {
            pdpConfig = balana.getPdpConfig();
        }

        // registering new attribute finder. so default PDPConfig is needed to change
        AttributeFinder attributeFinder = pdpConfig.getAttributeFinder();
        List<AttributeFinderModule> finderModules = attributeFinder.getModules();
        finderModules.add(new SampleAttributeFinderModule());
        attributeFinder.setModules(finderModules);

        return new PDP(new PDPConfig(attributeFinder, pdpConfig.getPolicyFinder(), null, true));
    }


    /**
     * Creates DOM representation of the XACML request
     *
     * @param response  XACML request as a String object
     * @return XACML request as a DOM element
     */
    public Element getXacmlResponse(String response) {

        ByteArrayInputStream inputStream;
        DocumentBuilderFactory dbf;
        Document doc;

        inputStream = new ByteArrayInputStream(response.getBytes());
        dbf = DocumentBuilderFactory.newInstance();
        dbf.setNamespaceAware(true);

        try {
            doc = dbf.newDocumentBuilder().parse(inputStream);
        } catch (ParserConfigurationException | SAXException | IOException e) {
            log.error("DOM of request element can not be created from String\n" + e);
            return null;
        } finally {
            try {
                inputStream.close();
            } catch (IOException e) {
               log.error("Error in closing input stream of XACML response\n" + e);
            }
        }
        return doc.getDocumentElement();
    }
    
    public synchronized static void initBalana(){
        Date date = new Date();
        long current_time = date.getTime();
        if(current_time - last_time <= DIFF_TIME)
            return;
        last_time = current_time;
        try{
            // using file based policy repository. so set the policy location as system property
            String policyLocation = (new File(".")).getCanonicalPath() + File.separator + "conf" + File.separator + "resources";
            log.info("Policy repository: " + policyLocation);
            System.setProperty(FileBasedPolicyFinderModule.POLICY_DIR_PROPERTY, policyLocation);
        } catch (IOException e) {
            log.error("Can not locate policy repository\n" + e);
        }
        // create default instance of Balana
        balana = Balana.getInstance();
    }
    
    public String createXACMLRequest(){
        String retval = "<Request xmlns=\"urn:oasis:names:tc:xacml:3.0:core:schema:wd-17\" CombinedDecision=\"false\" ReturnPolicyIdList=\"false\">" +
                "<Attributes Category=\"urn:oasis:names:tc:xacml:3.0:attribute-category:action\">" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:action:action-id\" IncludeInResult=\"false\">" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">"+ action +"</AttributeValue>" +
                "</Attribute>" +
                "</Attributes>" +
                "<Attributes Category=\"urn:oasis:names:tc:xacml:1.0:subject-category:access-subject\">" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:subject:subject-id\" IncludeInResult=\"false\">" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">"+ username +"</AttributeValue>" +
                "</Attribute>" +
                "</Attributes>" +
                "<Attributes Category=\"urn:oasis:names:tc:xacml:3.0:attribute-category:resource\">" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:resource:resource-id\" IncludeInResult=\"true\">" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">" + tableResource + "</AttributeValue>" +
                "</Attribute>" +
                "</Attributes>";
        for (String row : attrResource) {
            retval += "<Attributes Category=\"" + uri_column + "\">" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:resource:resource-id\" IncludeInResult=\"true\">" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">" + row.trim() + "</AttributeValue>" +
                "</Attribute>" +
                "</Attributes>";
        }
        retval += "</Request>";
        return retval;

    }
}
