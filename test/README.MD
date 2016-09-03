### [Create Fixture](https://www.npmjs.com/package/factory-mysql-fixtures)

```
npm install factory-mysql-fixtures
```

Example script:
```
var Factory = require('factory-mysql-fixtures');
 
var dbConf = {
  host: 'localhost',
  user: 'root',
  database: 'db-test',
  password: 'db-password', // optional 
  timezone: 'utc',
  dbStructureFile: './build/structure.sql' // set your own script to reload initial data when calling clean method 
};
  
Factory.config(dbConf);

var _nameIndex = 0;
Factory.define('person', { 
  name: function(cb) { cb('Jack - ' + _nameIndex++ ); } // lazy attribute 
  email: 'jack@mail.com' 
});
 
Factory.define('device', { 
  hash: 'hash1', 
  person_id: Factory.assoc('person', 'id') // simply Factory.assoc('person') for person object itself 
});

Factory.create('person', { name: 'Fred', email: 'fred@mail.com' }, function(err, result) {
  // result is the saved person id 
});
 
Factory.create('device', null, function(err, result) {
  // result is the saved device id 
});

Factory.clean(function(err) {
  // data reloaded 
});
```

# [RANDOM NAME for N](http://random-name-generator.info/random/?n=10&g=1&st=2)
