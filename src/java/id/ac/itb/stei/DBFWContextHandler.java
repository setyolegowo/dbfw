/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package id.ac.itb.stei;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
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

    private Balana balana;
    private String username;
    private String action;
    private String tableResource;
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
                    if(AbstractResult.DECISION_PERMIT == _result.getDecision()){
                        this.result = "ok";
                        Set<Attributes> attributesSet = ((Result)_result).getAttributes();
                        for(Attributes attributes : attributesSet){
                            for(Attribute attribute : attributes.getAttributes()){
                                resultColumns.add(attribute.getValue().encode());
                            }
                        }
                    } else if(AbstractResult.DECISION_INDETERMINATE == _result.getDecision()){
                        this.result = "ok";
                    } else {
                        this.result = "no";
                    }
                }
            } catch (ParsingException e) {
            }

        } else {
            System.err.println("\nUser name, action, and table resource can not be empty\n");
        }
    }
    
    /**
     * Returns a new PDP instance with new XACML policies
     *
     * @return a  PDP instance
     */
    private PDP getPDPNewInstance(){

        PDPConfig pdpConfig = balana.getPdpConfig();

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
            System.err.println("DOM of request element can not be created from String");
            return null;
        } finally {
            try {
                inputStream.close();
            } catch (IOException e) {
               System.err.println("Error in closing input stream of XACML response");
            }
        }
        return doc.getDocumentElement();
    }
    
    private void initBalana(){

        try{
            // using file based policy repository. so set the policy location as system property
            String policyLocation = (new File(".")).getCanonicalPath() + File.separator + "conf\\resources";
            System.setProperty(FileBasedPolicyFinderModule.POLICY_DIR_PROPERTY, policyLocation);
        } catch (IOException e) {
            System.err.println("Can not locate policy repository");
        }
        // create default instance of Balana
        balana = Balana.getInstance();
    }
    
    public String createXACMLRequest(){
        String retval = "<Request xmlns=\"urn:oasis:names:tc:xacml:3.0:core:schema:wd-17\" CombinedDecision=\"false\" ReturnPolicyIdList=\"false\">\n" +
                "<Attributes Category=\"urn:oasis:names:tc:xacml:3.0:attribute-category:action\">\n" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:action:action-id\" IncludeInResult=\"false\">\n" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">"+ action +"</AttributeValue>\n" +
                "</Attribute>\n" +
                "</Attributes>\n" +
                "<Attributes Category=\"urn:oasis:names:tc:xacml:1.0:subject-category:access-subject\">\n" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:subject:subject-id\" IncludeInResult=\"false\">\n" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">"+ username +"</AttributeValue>\n" +
                "</Attribute>\n" +
                "</Attributes>\n" +
                "<Attributes Category=\"urn:oasis:names:tc:xacml:3.0:attribute-category:resource\">\n" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:resource:resource-id\" IncludeInResult=\"true\">\n" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">" + tableResource + "</AttributeValue>\n" +
                "</Attribute>\n" +
                "</Attributes>\n";
        for (String row : attrResource) {
            retval += "<Attributes Category=\"urn:oasis:names:tc:xacml:3.0:attribute-category:column\">\n" +
                "<Attribute AttributeId=\"urn:oasis:names:tc:xacml:1.0:resource:resource-id\" IncludeInResult=\"true\">\n" +
                "<AttributeValue DataType=\"http://www.w3.org/2001/XMLSchema#string\">" + row + "</AttributeValue>\n" +
                "</Attribute>\n" +
                "</Attributes>\n";
        }
        retval += "</Request>";
        return retval;

    }
}
