
// Function to switch tabs
function openTab(evt, tabName, tabClassId, tabLinkId) {
	var tCont = document.getElementsByClassName(tabClassId);
	for (var i = 0; i < tCont.length; i++) {
		tCont[i].style.display = "none";
	}
	var tLink = document.getElementsByClassName(tabLinkId);
	for (i = 0; i < tLink.length; i++) {
		tLink[i].className = tLink[i].className.replace(" active", "");
	}
	document.getElementById(tabName).style.display = "block";
	evt.currentTarget.className += " active";
}
// Clear information functions
function showEmptyList(listName) {
	var name;
	if (listName == "descList") {
		name = "Descendants list empty";
	} else if (listName == "ancList") {
		name = "Ancestors list empty"
	} else {
		name = "Status list empty";
	}
	var node = document.createElement("li");
	node.className = "list-group-item";
	node.appendChild(document.createTextNode(name));
	document.getElementById(listName).appendChild(node);
}
function showEmptyTable(tableName) {
	if (!tableName) {
		return;
	}
	var tableBody = document.getElementById(tableName).getElementsByTagName('tbody')[0];
	var row = tableBody.insertRow(-1);
	row.innerHTML = "Table is empty";
	row.className = "empty-attribute";
}

function clearTable(tableName) {
	var rowNum = 1;
	var table = document.getElementById(tableName);
	var maxRow = table.rows.length;
	for (var i = rowNum; i < maxRow; i++) {
		table.deleteRow(rowNum);
	}
}
function clearCreateForm() {
	$('#cFilename').val('');
	$('#cSource').val('');
	$('#cGEDC').val('');
	$('#cSubName').val('');
	$('#cSubName').val('');
	$('#cSubAddr').val('');
}
function clearAddForm() {
	$('#addFirstname').val('');
	$('#addSurname').val('');
}
function clearList(listName) {	
	var list = document.getElementById(listName);
	while(list.firstChild) {
		list.removeChild(list.firstChild);
	}
}
// ---------------------------

// update data functions
function updateStatus(newStatus) {
	if ($(statusList).has("li").length == 1 && $(statusList).first().text() == "Status list empty") {
		var list = document.getElementById("statusList");
		list.removeChild(list.firstChild);
	}
	var node = document.createElement("li");
	node.className = "list-group-item";
	if (newStatus.includes("SUCCESS")) {
		node.style.color = "green";
	} else if (newStatus.includes("INFO")){
		node.style.color = "blue";
	} else if (newStatus.includes("WARNING")) {
		node.style.color = "orange";
	} else if (newStatus.includes("DATABASE")) {
		node.style.color = "purple";
	} else {
		node.style.color = "red";
	}
	node.appendChild(document.createTextNode(newStatus));
	document.getElementById("statusList").appendChild(node);
}
// ---------------------------

// popluate data functions
function populateFileTable(files) {
	if (!files || !files.fileInfoArray) {
		return;
	}
	var tableBody = document.getElementById("fileTable").getElementsByTagName('tbody')[0];
	for (var i = 0; i < files.fileInfoArray.length; i++) {
		var row = tableBody.insertRow(-1);
		var filesObj = files.fileInfoArray[i];
		for (var element in filesObj) {
			var cell = document.createElement("td");
			if (element == 'fileName') {
				cell.title = "click to download";
				let sub_name = filesObj[element].substring(10);
				let download_link = document.createElement("a");
				download_link.innerHTML = sub_name;
				download_link.href = filesObj[element];
				download_link.download = sub_name;
				cell.appendChild(download_link);
			} else {
				cell.innerHTML = filesObj[element];
			}
			row.appendChild(cell);			
		}		
	}
}
function populateIndTable(people) {
	var tableBody = document.getElementById("indTable").getElementsByTagName('tbody')[0];
	if (tableBody.firstChild && tableBody.firstChild.innerHTML == "Table is empty") {
		clearTable("indTable");
	}
	for (var i = 0; i < people.indObj.length; i++) {
		var row = tableBody.insertRow(-1);
		var person = people.indObj[i];
		for (var element in person) {
			var cell = row.insertCell(-1);
			cell.innerHTML = person[element];
		}
	}
}
function populateGenList(list, listName) {
	if (!list || !list.generations || !listName) {
		return;
	}
	for (var i = 0; i < list.generations.length; i++) {
		var gen = list.generations[i];
		var node = document.createElement("li");
		node.className = "list-group-item";
		var personNames = '';
		for (var element in gen) {	
			var person = gen[element];			
			for (var item in person) {
				personNames += ' ';
				personNames += person[item];			
			}
			personNames += ', ';
		}
		personNames = personNames.substring(0, personNames.length - 2);
		node.appendChild(document.createTextNode(personNames));
		document.getElementById(listName).appendChild(node);
	}
}
function populateFileOptions(files) {
	if (!files || !files.fileInfoArray) {
		return;
	}
	var fileListIDs = ["vSelect", "addSelect", "descSelect", "ancSelect", "qSelect"];
	for (var fileOpt of fileListIDs) {
		$('#' + fileOpt).find('option').remove().end().append('<option disabled selected hidden value>select a file</option>');	
	}
	for (var i = 0; i < files.fileInfoArray.length; i++) {		
		var file = files.fileInfoArray[i];
		if (file.fileName.substring(10)) {
			let fileNameStr = file.fileName.substring(10);
			// for(var j = 0; j < fileListIDs.length; j++) {
			for (var fileID of fileListIDs) {
				var select = document.getElementById(fileID);
				var element = document.createElement('option');
				element.className = "list-group-item";
				element.appendChild(document.createTextNode(fileNameStr));
				select.appendChild(element);
			}
		}		
	}
}
function populateSelectOptions(people, selectName) {
	if (!people || !people.indObj || !selectName) {
		return;
	}
	$('#' + selectName).find('option').remove().end().append('<option disabled selected hidden value>select an option</option>');
	for (var i = 0; i < people.indObj.length; i++) {
		var person = people.indObj[i];
		var fullName = person.givenName +" "+ person.surname;
		var element = document.createElement('option');
		element.className = "list-group-item";
		element.appendChild(document.createTextNode(fullName));
		document.getElementById(selectName).appendChild(element);
	}
}
// ---------------------------

// Create data functions
function createLoginObj() {
	let credentials = new Object();
	if ( $.trim( $('#userID').val() ) == '') {
		updateStatus("WARNING - Invalid username");
		return null;
	} else {
		credentials.userID = $('#userID').val();
	}
	if ( $.trim( $('#userPass').val() ) == '') {
		updateStatus("WARNING - Invalid password");
		return null;
	} else {
		credentials.userPass = $('#userPass').val();
	}
	if ( $.trim( $('#dbName').val() ) == '') {
		updateStatus("WARNING - Invalid Database");
		return null;
	} else {
		credentials.dbName = $('#dbName').val();
	}
	return credentials;
}
function createGedcomObj() {
	let obj = new Object();
	if ( $.trim( $('#cSource').val() ) == '') {
		updateStatus("FAILURE - Invalid source");
		return null;
	} else {
		obj.source = $('#cSource').val();
	}
	if ( $.trim( $('#cGEDC').val() ) == '' || isNaN($('#cGEDC').val())) { 
		updateStatus("FAILURE - Invalid GEDC version");
		return null;
	} else {
		obj.gedcVersion = $('#cGEDC').val();
	}
	obj.encoding = $('#cEncoding :selected').text();
	if ( $.trim( $('#cSubName').val() ) == '') { 
		updateStatus("FAILURE - Invalid Submitter name");
		return null;
	} else { 
		obj.subName = $('#cSubName').val();
	}
	
	if ( $.trim( $('#cSubAddr').val() ) == '') { 
		updateStatus("FAILURE - Invalid Submitter name");
		return null;
	} else {
		obj.subAddress = $('#cSubAddr').val();
	}
	return obj;
}
function createIndObj() {
	let person = new Object();
	if ( $.trim( $('#addFirstname').val() ) == '') {
		person = null;		
		updateStatus("FAILURE - Invalid first name");
	} else if ($.trim( $('#addSurname').val() ) == '' ) {
		person = null;	
		updateStatus("FAILURE - Invalid last name");
	} else {
		person.givenName = $('#addFirstname').val();
		person.surname = $('#addSurname').val();
	}
	return person;
}

// reload data functions
function loadFiles() {
	$.ajax({ // Get request for files
		type: 'get', dataType: 'json', url: '/files',
		success: function (data) {
			clearTable("fileTable");
			if (data == null || data == {} || jQuery.isEmptyObject(data.fileInfoArray)) {
				updateStatus("INFO - No files on server");
				showEmptyTable("fileTable");
			} else {
				populateFileTable(data);
				populateFileOptions(data);
			}
		}, fail: function(error) {
			updateStatus("FAILURE - Could not retrieve files");
		}
	});
}
function loadIndividuals(fName) {
	$.ajax({
		type: 'get', dataType: 'json', url: '/individuals/' + fName,
		success: function (data) {
			clearTable('indTable');
			if (data == null || data == {} || jQuery.isEmptyObject(data.indObj)) {
				updateStatus("INFO - No family members in file: " + fName);
				showEmptyTable("indTable");
			} else {
				populateIndTable(data);
			}
		}, fail: function(error) {
			updateStatus("FAILURE - Could not retrieve family members: " + fName);
		}
	});
}
// ---------------------------

function showDatabaseButtons() {
	var tCont = document.getElementsByClassName('hiddenContent');
	for (var i = 0; i < tCont.length; i++) {
		tCont[i].style.display = "block";
	}
	document.getElementById('loginBtn').style.display = "none";
}

function hideOtherQueries() {
	$("#query2").hide();
	$("#query3").hide();
	$("#query4").hide();
	$("#query5").hide();
	$("#query6").hide();
}

function showQueryForm() {
	var x = document.getElementById("querySelectGroup").selectedIndex;
	var y = document.getElementById("querySelectGroup").options;

	hideOtherQueries();
	
	switch (y[x].index +1) {
		case 1:
			break;
		case 2:
			$("#query2").show();
			break;
		case 3:
			$("#query3").show();
			break;
		case 4:
			$("#query4").show();
			break;
		case 5:
			$("#query5").show();
			break;
		case 6:
			$("#query6").show();
			break;
		default:
			break;
	}
}

function createQuery() {
	var x = document.getElementById("querySelectGroup").selectedIndex;
	var y = document.getElementById("querySelectGroup").options;
	switch (y[x].index) {
		case 0:
			return "select * from INDIVIDUAL order by surname;";
			break;
		case 1:
			return "select * from INDIVIDUAL where source_file = (select file_id from FILE where file_name = '" + $('#qSelect :selected').text() + "');";
			break;
		case 2:
			var sex = $('#qSexSelect :selected').text();
			if (sex == 'Female') {
				return "select * from INDIVIDUAL where sex = 'F';";
			} else if (sex == 'Male') {
				return "select * from INDIVIDUAL where sex = 'M';";
			} else {
				return "select * from INDIVIDUAL where sex = 'U';";
			}
			break;
		case 3:
			return "select * from INDIVIDUAL where fam_size > " + $('#famSizeQuery').val() + ";";
			break;
		case 4:
			return "select count(case when sex = 'M' then 1 end) as Males, count(case when sex = 'F' then 1 end) as Females, count(case when sex = 'U' then 1 end) as Unknown from INDIVIDUAL;"; 
			break;
		case 5:
			return $('#customQuery').val();
			break;
		default:
			break;
	}
}

function clearDBTable() {
	clearTable("queryTable");
	var table = document.getElementById("queryTable");
	var maxRow = table.rows.length;
	if (maxRow != 0) {
		table.deleteRow(0);
	}
}

function populateDBTable(queryData) {
	
	// console.log(queryData);
	clearDBTable();
	var table = document.getElementById("queryTable");
	var tableBody = document.getElementById("queryTable").getElementsByTagName('tbody')[0];	

	var qData = queryData.data;
	var tableHeaders = Object.keys(qData[0]);
	
	var headerRow = tableBody.insertRow(-1);
	for (var header in tableHeaders) {
		var cell = headerRow.insertCell(-1);
		cell.innerHTML = tableHeaders[header];
		cell.style.color = "blue";
	}
	
	for (var data in qData) {
		var row = tableBody.insertRow(-1);
		for (var element in qData[data]) {
			var cell = row.insertCell(-1);
			cell.innerHTML = qData[data][element];
		}
	}
}

function populateHelpTable(queryData) {
	
	var table = document.getElementById("queryTable");
	var maxRow = table.rows.length;
	if (maxRow != 0) {
		table.deleteRow(0);
	}
	var tableBody = document.getElementById("queryTable").getElementsByTagName('tbody')[0];
	
	var fileTableTitle = table.insertRow(-1);
	var cell = fileTableTitle.insertCell(-1);
	cell.innerHTML = "FILE Table";
	cell.style.color = "green";

	var fData = queryData.fileData;
	var tableHeaders = Object.keys(fData[0]);
	var headerRow = table.insertRow(-1);
	for (var header in tableHeaders) {
		var cell = headerRow.insertCell(-1);
		cell.innerHTML = tableHeaders[header];
		cell.style.color = "blue";
	}
	
	for (var data in fData) {
		// console.log(fData[data]);
		var row = tableBody.insertRow(-1);
		for (var element in fData[data]) {
			var cell = row.insertCell(-1);
			cell.innerHTML = fData[data][element];
		}
	}

	var indTableTitle = tableBody.insertRow(-1);
	var iCell = indTableTitle.insertCell(-1);
	iCell.innerHTML = "INDIVIDUAL Table";
	iCell.style.color = "green";

	var iData = queryData.indData;
	var iTableHeaders = Object.keys(iData[0]);
	var iHeadRow = tableBody.insertRow(-1);
	for (var header in iTableHeaders) {
		var cell = iHeadRow.insertCell(-1);
		cell.innerHTML = iTableHeaders[header];
		cell.style.color = "blue";
	}
	
	for (var data in iData) {
		var row = tableBody.insertRow(-1);
		for (var element in iData[data]) {
			var cell = row.insertCell(-1);
			cell.innerHTML = iData[data][element];
		}
	}
}

function callDBStatus() {
	console.log("getting status");
	$.ajax({
		url: '/displayDBStatus/', type: 'get', dataType: 'json',
		contentType: "application/json; charset=utf-8",
		success: function (data) {
			console.log(data);
			if (data.success == "SUCCESS") {
				updateStatus("DATABASE - Database has " + data.numFiles + " files and " +  data.numInd + " individuals");
			} else {
				updateStatus("DATABASE - Failed to update status");
			}
		}, fail: function(error) {
			updateStatus("DATABASE - Failed to update status");
		}
	});
}

$(document).ready(function() {	

// ********** A4 function calls *****************************************************

	$('#loginFormBtn').click(function (e) {
		e.preventDefault();
		let credentials = createLoginObj();
		if (credentials != null) {
			let json = JSON.stringify(credentials);
			$.ajax({
				url: '/login/', type: 'post', dataType: 'json',	data: json,
				contentType: "application/json; charset=utf-8",
				success: function (result) {
					if (result.success == "SUCCESS") {
						updateStatus("DATABASE - logged in to durslet.socs.uoguelph.ca");
						showDatabaseButtons();						
						callDBStatus();
						document.getElementById('clearDataBtn').style.display = "none";
					} else {
						updateStatus("DATABASE - Failed to login");	
					}
				}, fail: function(error) {
					updateStatus("DATABASE - Failed to login");
				}
			});
		}
	});	

	$('#execQueryBtn').click(function () {
		var queryStr = createQuery();
		$.ajax({
			url: '/execQuery/'+queryStr, type: 'get', dataType: 'json',
			contentType: "application/json; charset=utf-8",
			success: function (result) {
				if (result.success) {
					document.getElementById('qTableHolder').style.display = "block";
					clearDBTable();
					if (result.data.length != 0) {
						populateDBTable(result);
					} else {
						updateStatus("DATABASE - No results found");	
					}
				} else {
					updateStatus("DATABASE - Query could not be executed");	
				}
			}, fail: function(error) {
				updateStatus("DATABASE - Query could not be completed");
			}
		});
	});

	$('#storeFilesBtn').click(function () {
		$.ajax({
			url: '/storeAllFiles/', type: 'post', dataType: 'json',
			contentType: "application/json; charset=utf-8",
			success: function (result) {
				console.log(result);
				if (result.success == "SUCCESS") {
					updateStatus("DATABASE - Files were stored successfully");
					document.getElementById('storeFilesBtn').style.display = "none";
					document.getElementById('clearDataBtn').style.display = "block";
					callDBStatus();
				} else if (result.success == "EMPTY") {
					updateStatus("DATABASE - No files to store");
				} else {
					updateStatus("DATABASE - Files were not stored");
				}
			}, fail: function(error) {
				updateStatus("DATABASE - Files were not stored");
			}
		});
	});

	$('#clearDataBtn').click(function () {
		$.ajax({
			url: '/clearAllData/', type: 'post', dataType: 'json',
			contentType: "application/json; charset=utf-8",
			success: function (data) {
				console.log(data);
				if (data.success) {
					updateStatus("DATABASE - Database has 0 files and 0 individuals");
					document.getElementById('storeFilesBtn').style.display = "block";
					document.getElementById('clearDataBtn').style.display = "none";
				} else {
					updateStatus("DATABASE - Tables were not cleared");
				}
			}, fail: function(error) {
				updateStatus("DATABASE - Tables were not cleared");
			}
		});
	});

	$('#dbStatusBtn').click(function () {
		// console.log("Getting DB status");
		callDBStatus();
	});

	$('#helpBtn').click(function () {
		$.ajax({
			url: '/describeHelp/', type: 'get', dataType: 'json',
			contentType: "application/json; charset=utf-8",
			success: function (result) {
				if (result.success) {
					document.getElementById('qTableHolder').style.display = "block";
					clearDBTable();
					populateHelpTable(result);						
				} else {
					updateStatus("DATABASE - Failed to get table descriptions");
				}
			}, fail: function(error) {
				updateStatus("DATABASE - Failed to get table descriptions");
			}
		});
	});

	$('#querySelectGroup').change(function () {
		showQueryForm();
	});

// *************************************************************************

	$('#createForm').submit(function(e){ // create gedcom file form event listener
		e.preventDefault();
		var fileName = $('#cFilename').val();
		var extension = ".ged";
		if (fileName.indexOf(extension) !== -1) {
			let gedcom = createGedcomObj();
			if (gedcom != null) {
				let json = JSON.stringify(gedcom);
				$.ajax({
					url: '/create/' + fileName, type: 'post', dataType: 'json',	data: json,
					contentType: "application/json; charset=utf-8",
					success: function (data) {
						if (data == null || data == {} || jQuery.isEmptyObject(data.fileInfo)) {
							updateStatus("FAILURE - Invalid GEDCOM: " + fileName);
						} else {
							clearCreateForm();
							loadFiles();
							updateStatus("SUCCESS - Created new GEDCOM: "+fileName);
						}
					}, fail: function(error) {
						updateStatus("FAILURE - Could not create new GEDCOM");
					}
				});
			}
		} else {
			updateStatus("FAILURE - Invalid file name");
		}
	});

	$('#addForm').submit(function(e){ // add individual to file form event listener
		e.preventDefault();
		var fName = $('#addSelect :selected').text();
		let ind = createIndObj();
		if (ind == null) {
			return;
		} else if (!fName) {
			updateStatus("FAILURE - No file selected");
		} else {
			let json = JSON.stringify(ind);
			$.ajax({
				url: '/addInd/'+fName,type:'post',dataType:'json', data: json,
				contentType: "application/json; charset=utf-8",
				success: function (data) {
					if (data == null || data == {} || jQuery.isEmptyObject(data.indObj)) {
						updateStatus("FAILURE - Invalid individual");	
					} else {						
						clearAddForm();
						loadFiles();
						loadIndividuals(fName);
						updateStatus("SUCCESS - Added new individual: "+ind.givenName+" "+ind.surname);
					}
				}, fail: function(error) {
					updateStatus("FAILURE - Unable to add new individual");
				}
			});
		}
	});

	$('#descForm').submit(function(e){ // get descendants form event listener
		e.preventDefault();
		var fName = $('#descSelect :selected').text();
		var pName = $('#descIndSelect :selected').text();
		var genN = $('#numDesc').val();
		if (jQuery.isEmptyObject(fName) || jQuery.isEmptyObject(pName)) {
			updateStatus("WARNING - Invalid descendants form");
		} else {
			$.ajax({
				type: 'get', dataType: 'json', url:'/descendants/'+fName+'/'+pName+'/'+genN,
				success: function (data) {
					clearList('descList');
					if (data == null || data == {} || data.generations == {} || jQuery.isEmptyObject(data.generations)) {
						showEmptyList('descList');
						updateStatus("INFO - No descendants found for: " + pName);
					} else {
						populateGenList(data, 'descList');
						updateStatus("SUCCESS - Retrieved descendants of: " + pName);
					}
				}, fail: function(error) {
					updateStatus("FAILURE - Cannot retrieve descendants for: " + pName);
				}
			});
		}
	});

	$('#ancForm').submit(function(e){ // get ancestors form event listener
		e.preventDefault();
		var fileName = $('#ancSelect :selected').text();
		var pName = $('#ancIndSelect :selected').text();
		var genN = $('#numAnc').val();
		if (jQuery.isEmptyObject(fileName) || jQuery.isEmptyObject(pName)) {
			updateStatus("WARNING - Invalid ancestors form");
		} else {
			$.ajax({
				type: 'get', dataType: 'json', url:'/ancestors/'+fileName+'/'+pName+'/'+genN,
				success: function (data) {
					clearList('ancList');
					if (data == null || data == {} || jQuery.isEmptyObject(data.generations)) {
						showEmptyList('ancList');
						updateStatus("INFO - No ancestors found for: " + pName);
					} else {
						populateGenList(data, 'ancList');
						updateStatus("SUCCESS - Retrieved ancestors of: " + pName);
					}
				}, fail: function(error) {
					updateStatus("FAILURE - Cannot retrieve ancestors for: " + pName);
				}
			});
		}
	});
	
	$('#clearBtn').click(function(){ // clear view panel list
		clearList('statusList');
		showEmptyList('statusList');
	});
	
	$('#vSelect').change(function(){ // update GEDCOM view
		var fName = $('#vSelect :selected').text();
		if (fName && fName != '') {
			loadIndividuals(fName);
		}
	});
	
	$('#descSelect').change(function(){ // update indi list from file dropdown
		var fName = $('#descSelect :selected').text();
		$.ajax({
			type: 'get', dataType: 'json', url: '/individuals/'+fName,
			success: function (data) {
				$('#descIndSelect').empty();
				if (data == null || data == {} || jQuery.isEmptyObject(data.indObj)) {
					updateStatus("INFO - No family members in file: " + fName);
				} else {
					populateSelectOptions(data, 'descIndSelect');
				}
			}, fail: function(error) {
				updateStatus("FAILURE - Could not retrieve family members");
			}
		});
	});

	$('#ancSelect').change(function(){
		var fileName = $('#ancSelect :selected').text();
		$.ajax({
			type: 'get', dataType: 'json', url: '/individuals/' + fileName,
			success: function (data) {
				if (data == null || data == {} || jQuery.isEmptyObject(data.indObj)) {
					$('#ancIndSelect').empty();
					updateStatus("INFO - No family members in file: " + fileName);
				} else {
					populateSelectOptions(data, 'ancIndSelect');
				}
			}, fail: function(error) {
				updateStatus("FAILURE - Could not retrieve family members");
			}
		});
	});

	// On start functions
	showEmptyTable("indTable");
	showEmptyList('statusList');
	hideOtherQueries();
	loadFiles();

	document.getElementById('qTableHolder').style.display = "none";
	
});
