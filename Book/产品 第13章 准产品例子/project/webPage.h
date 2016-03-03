//使用PROGMEM定义到代码区
const char mainPageString[] PROGMEM="<html><head><title>ESPduino Setting</title>\r\n\
<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\r\n\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=0\">\r\n\
<style>\r\n\
body{max-width:640px;width:100%;margin:0 auto;font-family: Arial,sans-serif;font-size: 15px;}\r\n\
input,select,button{font-family:inherit;font-size:inherit;}\r\n\
form{background:#f7f7f7;width:90%;margin: 1em auto;line-height: 2.4em;padding:5%;box-shadow: 0 0 10px rgba(0,0,0,.5);border-radius: 10px;}\r\n\
.button_group{text-align:center;margin-top: 1em;}\r\n\
.control_group label{width:45%;display: inline-block;text-align:right;padding-right: 5%;}\r\n\
.control_group input{max-width:45%;}\r\n\
.co{ color:#000080;}\r\n\
.bk{ background-color:#F0F0F0;}\r\n\
button{border:none;border-radius:4px;line-height:2em;color:#fff;margin-right: 1em;}\r\n\
button.green{background:#45c01a;}\r\n\
button.gray{background:#666;}\r\n\
button.blue{background:#466;}\r\n\
</style>\r\n\
<script stype=\"text/javascript\">\r\n\
var xmlAplist=null; \r\n\
var xmlSubmit=null;\r\n\
var xmlPara=null;\r\n\
function newxmlAplist(){\r\n\
xmlAplist= window.XMLHttpRequest? new XMLHttpRequest() : new ActiveXObject(\"Microsoft.XMLHttp\");\r\n\
}\r\n\
function newxmlSubmit(){\r\n\
xmlSubmit= window.XMLHttpRequest? new XMLHttpRequest() : new ActiveXObject(\"Microsoft.XMLHttp\");\r\n\
}\r\n\
function newxmlPara(){\r\n\
xmlPara= window.XMLHttpRequest? new XMLHttpRequest() : new ActiveXObject(\"Microsoft.XMLHttp\");\r\n\
}\r\n\
function retAplist()\r\n\
{if(xmlAplist.readyState == 4 && xmlAplist.status == 200){\r\n\
var res= xmlAplist.responseText;\r\n\
if(res.length>0){\r\n\
document.getElementById(\"sta_list\").options.length=0;\r\n\
var a_list=new Array();\r\n\
a_list=res.split(\"^$&\");\r\n\
for (i=0;i<a_list.length ;i++ ) {\r\n\
if(a_list[i].length>0){\r\n\
var t=a_list[i];\r\n\
document.getElementById(\"sta_list\").options.add(new Option(t,t));\r\n\
}}}}}\r\n\
function retSubmit()\r\n\
{if(xmlSubmit.readyState == 4 && xmlSubmit.status == 200){\r\n\
 alert(xmlSubmit.responseText);\r\n\
}}\r\n\
function retPara()\r\n\
{if(xmlPara.readyState == 4 && xmlPara.status == 200){\r\n\
  res= xmlPara.responseText;\r\n\
  var d=new Array();\r\n\
  d=res.split(\"^$&\");\r\n\
  document.getElementById(\"staip\").value=d[0];\r\n\
  document.getElementById(\"stamac\").value=d[1];\r\n\
  document.getElementById(\"id\").value=d[2];\r\n\
  document.getElementById(\"key\").value=d[3];\r\n\
  document.getElementById(\"ssid\").value=d[4];\r\n\
  document.getElementById(\"psw\").value=d[5];\r\n\
}}\r\n\
function get_ap_list(){\r\n\
newxmlAplist();\r\n\
xmlAplist.onreadystatechange = retAplist ;\r\n\
xmlAplist.open(\"GET\", \"/ap_list.do\", true);\r\n\
xmlAplist.send( null );\r\n\
}\r\n\
function get_para(){\r\n\
newxmlPara();\r\n\
xmlPara.onreadystatechange = retPara;\r\n\
xmlPara.open(\"GET\", \"/para_list.do\", true);\r\n\
xmlPara.send( null );\r\n\
}\r\n\
function submitForm() {\r\n\
  var ssid=document.getElementById('ssid').value;\r\n\
  var psw=document.getElementById('psw').value;\r\n\
  var id=document.getElementById('id').value;\r\n\
  var key=document.getElementById('key').value;\r\n\
  var para1=\"ssid='\"+ssid+\"',\" +\"psw='\"+psw+\"',\";\r\n\
  var para2=\"device_id='\"+id+\"',\" +\"device_key='\"+key+\"',\";\r\n\
  newxmlSubmit();\r\n\
  xmlSubmit.onreadystatechange = retSubmit;\r\n\
  xmlSubmit.open(\"POST\", \"/submit\", true);\r\n\
  xmlSubmit.send(para1+para2);\r\n\
  //alert(\"Submit Successfully\");\r\n\
}\r\n\
</script>\r\n\
</head>\r\n\
<body>\r\n\
<form name=\"hotform\" id=\"hotform\" action='' method=\"post\" class='co'>\r\n\
<div class=\"button_group\">\r\n\
<h2>ESPduino Setting</h2>\r\n\
</div>\r\n\
<label>WiFi Status: </label>\r\n\
<div class=\"control_group\">\r\n\
  <label >STA IP:</label><input class='bk' type='text' id='staip' name='staip' size='20' readonly />\r\n\
  <label>STA MAC:</label><input class='bk' type='text' id='stamac' name='stamac' size='20' readonly />\r\n\
</div>\r\n\
<label>Device ID&Key: </label>\r\n\
<div class=\"control_group\">\r\n\
  <label>Device ID:</label><input type='text' id='id' name='id' size='20' placeholder=\"Enter Device ID\"/>\r\n\
  <label>Device Key:</label><input type='text' id='key' name='key' size='20' placeholder=\"Enter Device Key\"/>\r\n\
</div>\r\n\
<label>STA Setting: </label>\r\n\
<div class=\"control_group\">\r\n\
  <label>STA SSID:</label><input type='text' id='ssid' name='ssid' size='20' placeholder=\"Enter SSID\" />\r\n\
  <label>AP List:</label><select name=\"sta_list\" id=\"sta_list\" style=\"width:160\" onchange=\"document.getElementById('ssid').value=this.value;\"/>\r\n\
  </select>\r\n\
  <div class=\"control_group\"><label>Scan:</label><button class=\"blue\" style=\"width:100\" onclick=\"get_ap_list();return false;\" >Refresh</button></div>\r\n\
  <label>STA Password:</label><input type='text' id='psw' name='psw' size='20' placeholder=\"Password\" />\r\n\
</div>\r\n\
<div class=\"button_group\">\r\n\
  <button class=\"green\" style=\"width:173\" onclick=\"submitForm();return false;\">Submit</button>\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
<a href='update.do'>Update Firmware</a><br>\r\n\
Visit <a href=\"http://bbs.doit.am/\">bbs.doit.am </a> for more help\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Copyright &copy; <a href=\"http://www.doit.am/\">www.doit.am </a> 2016(V1.0), All Rights Reserved\r\n\
</div>\r\n\
</form>\r\n\
<script type='text/javascript'>\r\n\
get_para();\r\n\
get_ap_list();\r\n\
</script></body></html>";

const char updatePageString[] PROGMEM="<html><head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'><title>ESPduino Setting</title>\r\n\
<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=0'>\r\n\
<style>\r\n\
body{max-width:640px;width:100%;margin:0 auto;font-family: Arial,sans-serif;font-size: 15px;}\r\n\
input,select,button{font-family:inherit;font-size:inherit;}\r\n\
form{background:#f7f7f7;width:90%;margin: 1em auto;line-height: 2.4em;padding:5%;box-shadow: 0 0 10px rgba(0,0,0,.5);border-radius: 10px;}\r\n\
.button_group{text-align:center;margin-top: 1em;}\r\n\
.control_group label{width:45%;display: inline-block;text-align:right;padding-right: 5%;}\r\n\
.control_group input{max-width:45%;}\r\n\
.co{ color:#000080;}\r\n\
.bk{ background-color:#F0F0F0;}\r\n\
input{border:none;border-radius:4px;line-height:2em;color:#fff;margin-right: 1em;}\r\n\
input.green{background:#45c01a;}\r\n\
input.gray{background:#666;}\r\n\
input.blue{background:#466;}\r\n\
</style></head><body>\r\n\
<form action='/update' method='post' enctype='multipart/form-data' class='co'>\r\n\
<div class='button_group'>\r\n\
<h2>Update Firmware</h2>\r\n\
</div>\r\n\
<label>Choose Binary File: </label>\r\n\
<div class='button_group'>\r\n\
  <input type='file' name='update' class='blue'>\r\n\
</div>\r\n\
<div class='button_group'>\r\n\
  <input type='submit' value='Update' class='green' style='width:173'>\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Be very carefully to choose the binary file. <br>\r\n\
Incorrect file may cause to board failed to start.<br>\r\n\
Visit <a href='http://bbs.doit.am/'>bbs.doit.am </a> for more help\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Copyright © <a href='http://www.doit.am/'>www.doit.am </a> 2016(V1.0), All Rights Reserved\r\n\
</div></form></body></html>";

const char updatePageString_ResultOK[] PROGMEM="<html><head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'><title>ESPduino Setting</title>\r\n\
<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=0'>\r\n\
<style>\r\n\
body{max-width:640px;width:100%;margin:0 auto;font-family: Arial,sans-serif;font-size: 15px;}\r\n\
input,select,button{font-family:inherit;font-size:inherit;}\r\n\
form{background:#f7f7f7;width:90%;margin: 1em auto;line-height: 2.4em;padding:5%;box-shadow: 0 0 10px rgba(0,0,0,.5);border-radius: 10px;}\r\n\
.button_group{text-align:center;margin-top: 1em;}\r\n\
.control_group label{width:45%;display: inline-block;text-align:right;padding-right: 5%;}\r\n\
.control_group input{max-width:45%;}\r\n\
.co{ color:#000080;}\r\n\
.bk{ background-color:#F0F0F0;}\r\n\
input{border:none;border-radius:4px;line-height:2em;color:#fff;margin-right: 1em;}\r\n\
input.green{background:#45c01a;}\r\n\
input.gray{background:#666;}\r\n\
input.blue{background:#466;}\r\n\
</style></head><body>\r\n\
<form action='/update' method='post' enctype='multipart/form-data' class='co'>\r\n\
<div class='button_group'>\r\n\
<h2>Update Firmware</h2>\r\n\
</div>\r\n\
<label>Upload result: </label>\r\n\
<div class='button_group'>\r\n\
  Upload sucessfully!\r\n\
  <br>\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Visit <a href='http://bbs.doit.am/'>bbs.doit.am </a> for more help\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Copyright © <a href='http://www.doit.am/'>www.doit.am </a> 2016(V1.0), All Rights Reserved\r\n\
</div></form></body></html>";

const char updatePageString_ResultFailed[] PROGMEM="<html><head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'><title>ESPduino Setting</title>\r\n\
<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=0'>\r\n\
<style>\r\n\
body{max-width:640px;width:100%;margin:0 auto;font-family: Arial,sans-serif;font-size: 15px;}\r\n\
input,select,button{font-family:inherit;font-size:inherit;}\r\n\
form{background:#f7f7f7;width:90%;margin: 1em auto;line-height: 2.4em;padding:5%;box-shadow: 0 0 10px rgba(0,0,0,.5);border-radius: 10px;}\r\n\
.button_group{text-align:center;margin-top: 1em;}\r\n\
.control_group label{width:45%;display: inline-block;text-align:right;padding-right: 5%;}\r\n\
.control_group input{max-width:45%;}\r\n\
.co{ color:#000080;}\r\n\
.bk{ background-color:#F0F0F0;}\r\n\
input{border:none;border-radius:4px;line-height:2em;color:#fff;margin-right: 1em;}\r\n\
input.green{background:#45c01a;}\r\n\
input.gray{background:#666;}\r\n\
input.blue{background:#466;}\r\n\
</style></head><body>\r\n\
<form action='/update' method='post' enctype='multipart/form-data' class='co'>\r\n\
<div class='button_group'>\r\n\
<h2>Update Firmware</h2>\r\n\
</div>\r\n\
<label>Upload result: </label>\r\n\
<div class='button_group'>\r\n\
  Upload Failed!\r\n\
  <br>\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Visit <a href='http://bbs.doit.am/'>bbs.doit.am </a> for more help\r\n\
</div>\r\n\
<div style='text-align:center;'>\r\n\
Copyright © <a href='http://www.doit.am/'>www.doit.am </a> 2016(V1.0), All Rights Reserved\r\n\
</div></form></body></html>";
