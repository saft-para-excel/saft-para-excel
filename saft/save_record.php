<?php
include_once 'database.php';
ini_set('display_errors', '1');
ini_set('display_startup_errors', '1');
error_reporting(E_ALL);


function save_program() {
    if ($_POST['certificate-num'] == -1){
        $sql = "INSERT INTO  programas (name,version,producer,certificate_num,status, date,name_link,producer_link) VALUES ( 
            '".$_POST['name']."',".$_POST['version'].$_POST['producer'].");";
        execute($sql);
    }
    else
    {
        $sql = sprintf("UPDATE  programas 
    set name ='%s', version='%s', producer='%s', name_link='%s', producer_link='%s' 
    where certificate_num='%s';",
            $_POST['name'], $_POST['version'], $_POST['producer'],
            $_POST['name-link'], $_POST['producer-link'],
            $_POST['certificate-num']);
        execute($sql);
    }
}
