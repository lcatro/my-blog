
<?php
    
    header('Accept-Charset: unicode');

    $receive_data_file=fopen('receive_data.txt','r');

    $receive_data=fread($receive_data_file,filesize('receive_data.txt'));
    fclose($receive_data_file);

    echo str_replace("\n",'<br/>',htmlspecialchars($receive_data));

?>
