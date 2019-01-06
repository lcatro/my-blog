
<?php

session_start();

if (!isset($_SESSION['username'])) {
    $_SESSION['username'] = 'root';
    
    echo 'Reflash for get tips .SID = ' . session_id();
    
    header('Set-Cookie: PHPSESSID=' . session_id() . '; max-age=a');  // PoC 1
} else {
    echo 'Your username is ' . $_SESSION['username'];
    
    session_destroy();
}

?>
