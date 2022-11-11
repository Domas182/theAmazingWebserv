<?php
if ($handle = opendir('.')) {
    while (false !== ($file = readdir($handle)))
    {
        if (($file != ".") 
         && ($file != "..") && ($file != "listing.php"))
        {
            $thelist .= '<LI><a href="'.$file.'">'.$file.'</a>';
        }
    }

    closedir($handle);
}
?>

<P>List of files:</p>
<UL>
<P><?=$thelist?></p>
</UL>