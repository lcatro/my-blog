
<?php

    if (isset($_POST['data'])) {
        $receive_data_file=fopen('receive_data.txt','a');
        
        fwrite($receive_data_file,$_POST['data']."\n");
        fclose($receive_data_file);
        
        $new_receive_cache_file=fopen('receive_cache.txt','a');
        
        fwrite($new_receive_cache_file,$_POST['data']."\n");
        fclose($new_receive_cache_file);

        echo 'OK';
    } else {
        echo 'ERROR';
    }

?>
