'use strict'

// C library API
const ffi = require('ffi');
// MySQL NPM API
const mysql = require('mysql');

// Custom Library API
let libparser = ffi.Library('./libparser', {
	'saveGEDCOM' : [ 'void', [ 'string', 'string' ] ],
	'saveIndividual' : [ 'bool', [ 'string', 'string' ] ],
	'getGenerations' : [ 'string', [ 'string', 'string', 'bool', 'int' ] ],
	'getFile' : [ 'string', [ 'string' ] ],
	'getIndividuals' : [ 'string', [ 'string' ] ],
});

// Express App (Routes)
const express 	 = require("express");
const app		 = express();
const path		 = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Send HTML at root, do not change
app.get('/',function(req,res){
	res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
	//Feel free to change the contents of style.css to prettify your Web app
	res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
	fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
		const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
		res.contentType('application/javascript');
		res.send(minimizedContents._obfuscatedCode);
	});
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
	// console.log(req.files);
	if(!req.files) {
		return res.status(400).send('No files were uploaded.');
	}
 
	let uploadFile = req.files.uploadFile;
 
	// Use the mv() method to place the file somewhere on your server
	uploadFile.mv('uploads/' + uploadFile.name, function(err) {
		if(err) {
			return res.status(500).send(err);
		}
		res.redirect('/');
	});
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
	fs.stat('uploads/' + req.params.name, function(err, stat) {
		// console.log(err);
		if(err == null) {
			res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
		} else {
			res.send('');
		}
	});
});

//******************** Your code goes here ******************** 

app.post('/create/:fileName', function(req, res) {
	if(!req.params || !req.params.fileName || !req.body) {
		return res.status(400).send('GEDCOM file not created');
	}
	let file = req.params.fileName;
	let gedcom = JSON.stringify(req.body);
	libparser.saveGEDCOM("./uploads/" + file, gedcom);
	var uploadedFiles = fs.readdirSync('./uploads/');
	var fileInfo = {};
	if (uploadedFiles.indexOf(file) !== -1) {
		fileInfo = libparser.getFile("./uploads/" + file);		
	}
	res.send({ fileInfo });
});

app.post('/addInd/:fileName', function(req, res) {
	if(!req.params || !req.params.fileName || !req.body) {
		return res.status(400).send('Ind not added');
	}
	let file = req.params.fileName;
	let person = JSON.stringify(req.body);
	let isSaved = libparser.saveIndividual("./uploads/" + file, person);
	var indObj = {};
	if (isSaved == true) {
		let individuals = libparser.getIndividuals("./uploads/" + file);
		if (individuals && individuals != "" && individuals != "{}" && individuals != {}) {
			indObj = JSON.parse(individuals);
		}
	}
	res.send({ indObj });
});

app.get('/files', function(req , res) {
	var uploadedFiles = fs.readdirSync('./uploads/');
	var fileInfoArray = [];
	uploadedFiles.forEach(file => {
		let fileInfo = libparser.getFile("./uploads/" + file);
		if (fileInfo != null && fileInfo != "{}") {
			fileInfoArray.push(JSON.parse(fileInfo));
		}
	});
	res.send({ fileInfoArray });
});

app.get('/individuals/:fileName', function(req , res){
	if(!req.params || !req.params.fileName) {
		return res.status(400).send('No individuals found');
	}
	let file = req.params.fileName;
	let individuals = libparser.getIndividuals("./uploads/" + file);
	var indObj = {};
	if (individuals != null && individuals != "" && individuals != "{}") {
		indObj = JSON.parse(individuals);
	}
	res.send({ indObj });
});

app.get('/descendants/:fName/:pName/:genN', function(req , res){
	if(!req.params || !req.params.fName || !req.params.pName || !req.params.genN) {
		return res.status(400).send('No descendants found');
	}
	let person = new Object();
	let personStr = req.params.pName;
	var keys = personStr.split(" ");
	if (!keys[0] && !keys[1]) {
		return res.status(400).send('Invalid name');
	} else {
		person.givenName = keys[0];
		person.surname = keys[1];
	}
	let file = req.params.fName;
	let numGen = req.params.genN;
	let personJson = JSON.stringify(person);
	let descJSON = libparser.getGenerations("./uploads/" + file, personJson, true, numGen);
	var generations = {};
	if (descJSON != null && descJSON != "" && descJSON != "{}") {
		generations = JSON.parse(descJSON);
	}
	res.send({ generations });
});

app.get('/ancestors/:fName/:pName/:genN', function(req , res){
	if(!req.params || !req.params.fName || !req.params.pName || !req.params.genN) {
		return res.status(400).send('No ancestors found');
	}
	let personObj = new Object();
	let personStr = req.params.pName;
	var keys = personStr.split(" ");
	if (!keys[0] || !keys[1]) {
		return res.status(400).send('Invalid name');
	} else {
		personObj.givenName = keys[0];
		personObj.surname = keys[1];
	}
	let file = req.params.fName;
	let numGen = req.params.genN;
	let pJSON = JSON.stringify(personObj);
	let ancsJSON = libparser.getGenerations("./uploads/" + file, pJSON, false, numGen);
	var generations = {};
	if (ancsJSON != null && ancsJSON != "" && ancsJSON != "{}") {
		generations = JSON.parse(ancsJSON);
	}
	res.send({ generations });
});

// **************** A4 function routes ******************************************

var connection = null;

let createFileQuery = "create table FILE (file_id int not null auto_increment,"
	+ "file_name varchar(60) not null, source varchar(250) not null,"
	+ "version varchar(10) not null, encoding varchar(10) not null,"
	+ "sub_name varchar(62) not null, sub_addr varchar(256) not null,"
	+ "num_individuals int, num_families int, primary key (file_id) );"

let createIndQuery = "create table INDIVIDUAL (ind_id int not null auto_increment,"
	+ "surname varchar(256) not null, given_name varchar(256) not null,"
	+ "sex varchar(1) not null, fam_size varchar(10) not null,"
	+ "source_file int, primary key (ind_id),"
	+ "foreign key(source_file) references FILE(file_id) on delete cascade);";
					
app.post('/login', function(req, res) {
	if(!req.params || !req.body) {
		return res.status(400).send('Bad Credentials');
	}
	
	var response = { success : 'SUCCESS' };
	
	let uid = req.body.userID;
	let upass = req.body.userPass;
	let db = req.body.dbName;
	console.log("User id:" + uid + " Pass: " + upass + " DB name: " + db);
	
	connection = mysql.createConnection({
		host:'dursley.socs.uoguelph.ca',
		user: uid,
		password: upass,
		database: db
	});

	connection.connect(function(err) {
		if (err) {
			response.success = 'FAILURE';
			res.send(JSON.stringify(response));
		} else {
			
			connection.query(createFileQuery, function (err, rows, fields) {
				if (err) console.log("Create file table err: " + err); 
			});
		
			connection.query(createIndQuery, function (err, rows, fields) {
				if (err) console.log("Create IND table: " + err);
			});
		
			connection.query("delete from FILE;", function (err, result) {
				if (err) {
					response.success = 'FAILURE';
				}
				res.send(JSON.stringify(response));
			});
		}
	});
});

app.post('/storeAllFiles', function (req, res) {
	var response = { success : 'SUCCESS' };
	var uploadedFiles = fs.readdirSync('./uploads/');
	if (uploadedFiles.length == 0) {
		console.log("empty");
		var response = { success : 'EMPTY' };
		res.send(JSON.stringify(response));
	} else {
		uploadedFiles.forEach(file => {
			let fObj = JSON.parse(libparser.getFile("./uploads/" + file));
			let insertFileQuery = "insert into FILE (file_id,file_name,source,version,encoding,sub_name,sub_addr,num_individuals,num_families) values (NULL,'" + file + "','" + fObj.source + "','" + fObj.gedcVersion + "','" + fObj.encoding + "','" + fObj.subName + "','" + fObj.subAddr + "'," + fObj.numInd + "," + fObj.numFam + ");";
			connection.query(insertFileQuery, function (err, result) {
				if (err) {
					response.success = 'FAILURE';
					console.log('failure');
					res.send(JSON.stringify(response));
				} else {
					var insertID = result.insertId;
					let iObj = JSON.parse(libparser.getIndividuals("./uploads/" + file));
					for (var j = 0; j < iObj.length; j++) {
						let insertIndQuery = "insert into INDIVIDUAL (ind_id,surname,given_name,sex,fam_size,source_file) values (NULL,'" + iObj[j].surname + "','" + iObj[j].givenName + "','" + iObj[j].sex + "'," + iObj[j].famSize + ",'" + insertID + "');";
						connection.query(insertIndQuery, function (err, result) {
							if (err) {
								response.success = 'FAILURE';
								console.log('failure');
							}							
						});
					}					
				}
			});	
		});	
	}
	res.send(JSON.stringify(response));
});

app.post('/clearAllData', function(req, res) {
	var response = { success : 'SUCCESS' };
	connection.query("delete from FILE;", function (err, result) {
		if (err) {
			response.success = 'FAILURE';
		}
		res.send(JSON.stringify(response));
	});

	// connection.query("delete from INDIVIDUAL;", function (err, result) {
	// 	if (err) {
	// 		response.success = 'FAILURE';
	// 		res.send(JSON.stringify(response));
	// 	} else {
	// 		connection.query("delete from FILE;", function (err, result) {
	// 			if (err) {
	// 				response.success = 'FAILURE';
	// 			}
	// 			res.send(JSON.stringify(response));
	// 		});	
	// 	}		
	// });
});

app.get('/displayDBStatus', function(req, res) {
	console.log('status...');
	let response = { success : 'SUCCESS', numFiles: 0, numInd: 0 };
	connection.query("select count(*) from FILE;", function (err, rows, fields) {
		if (err) {
			response.success = 'FAILURE';
			console.log(err);
		} else {
			response.numFiles = rows[0]['count(*)'];
			connection.query("select count(*) from INDIVIDUAL;", function (errB, rowsB, fieldsB) {
				if (errB) {
					response.success = 'FAILURE';
					console.log(errB);
				}
				response.numInd = rowsB[0]['count(*)'];		
				console.log('sending back');
				res.send(JSON.stringify(response));
			});
		}		
	});
});

app.get('/describeHelp', function(req, res) {
	var response = { success : 'SUCCESS' };
	connection.query("describe FILE;", function (err, rows, fields) {
		if (err) {
			response.success = 'FAILURE';
			res.send(JSON.stringify(response));
		}
		response.fileData = rows;
		connection.query("describe INDIVIDUAL;", function (err, rows, fields) {
			if (err) {
				response.success = 'FAILURE';
				res.send(JSON.stringify(response));
			}
			response.indData = rows;
			res.send(JSON.stringify(response));
		});
	});
});

app.get('/execQuery/:queryStr', function(req, res) {
	if(!req.params || !req.params.queryStr) {
		return res.status(400).send('Bad query');
	}
	var response = { success : 'SUCCESS' };
	let query = req.params.queryStr;
	connection.query(query, function (err, rows, fields) {
		if (err) {
			response.success = 'FAILURE';
		}
		response.data = rows;
		res.send(JSON.stringify(response));
	});
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
