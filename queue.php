<?php
$workfile = $_SERVER["argv"][1];
$url = $_SERVER["argv"][2];

$m = new MongoClient();
$web = $m->selectCollection('ksearch', 'webs');

$content =  file_get_contents($workfile);
//echo $content;
preg_match('/<title>.*<\/title>/i',$content,$title);
//print_r($title);
$title = str_replace("<title>","",$title[0]);
$title = str_replace("</title>","",$title);
try{
	$web->insert(array("url" => $url, "title" => $title, "mirror"=>"/opt/larbin/".$workfile,"ts" => new MongoDate()));
}catch(Exception $e){
	$title = iconv("GBK", "UTF-8", $title);
	//print $e->getMessage();
	try{
		$web->insert(array("url" => $url, "title" => $title, "mirror"=>"/opt/larbin/".$workfile,"ts" => new MongoDate()));
	}catch(Exception $e){
		print $e->getMessage();
	}
}