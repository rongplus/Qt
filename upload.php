<?php
if ($_FILES["file"]["error"] > 0)
  {
  echo "Error: " . $_FILES["file"]["error"] . "<br />";
  }
else
  {
  echo "Upload: " . $_FILES["file"]["name"] . "<br />";//被上传文件的名称 
  echo "Type: " . $_FILES["file"]["type"] . "<br />";//被上传文件的类型 
  echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";//被上传文件的大小，以字节计 
  echo "Stored in: " . $_FILES["file"]["tmp_name"];//存储在服务器的文件的临时副本的名称
  }
  if (file_exists("upload/" . $_FILES["file"]["name"]))
      {
      echo $_FILES["leedabao.txt"]["name"] . " already exists. ";
      }
    else
      {
      move_uploaded_file($_FILES["file"]["tmp_name"],"upload/" . $_FILES["file"]["name"]);
      echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
      }
?>