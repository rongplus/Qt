<?php
session_start();
echo $_SESSION['count'];
unset($_SESSION['count']);
?>