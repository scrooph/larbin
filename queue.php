<?php
$workfile = $_SERVER["argv"][1];
$url = $_SERVER["argv"][2];
$dbengine = 'mysql';
switch($dbengine){
	case 'mongodb':
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
	break;
	case 'mysql':
		$mysql_server_name="localhost"; //数据库服务器名称
		$mysql_username="test"; // 连接数据库用户名
		$mysql_password=""; // 连接数据库密码
		$mysql_database="test"; // 数据库的名字
		
		// 连接到数据库
		$conn=mysql_connect($mysql_server_name, $mysql_username,$mysql_password);
		$content =  file_get_contents($workfile);
		//echo $content;
		preg_match('/<title>.*<\/title>/i',$content,$title);
		print_r($title);
		$title = str_replace("<title>","",$title[0]);
		$title = str_replace("</title>","",$title);
		echo $title;
		mysql_select_db($mysql_database, $conn);
		$res = mysql_query("INSERT INTO documents (group_id, group_id2, date_added, title, content) VALUES (1, 5, NOW(), '".$title."', '".$content."')");
		//print_r($res);
		mysql_close($conn);
	break;
	default:
}