
<?php
    
    header('Accept-Charset: unicode');

    $new_receive_cache_file=fopen('receive_cache.txt','r');

    $new_receive_cache_data=fread($new_receive_cache_file,filesize('receive_cache.txt'));
    fclose($new_receive_cache_file);

    echo str_replace("\n",'<br/>',htmlspecialchars($new_receive_cache_data));
                          
    $new_receive_cache_file=fopen('receive_cache.txt','w');
    fclose($new_receive_cache_file);

?>
