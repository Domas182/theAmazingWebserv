<?php
if ($handle = opendir('.')) {
    while (false !== ($file = readdir($handle)))
    {
        if (($file != ".")
         && ($file != "..") && ($file != "listing.php") && strpos($file, ".") !== false)
        {
            $thelist .= '<LI><a href="'.$file.'">'.$file.'</a>';
        }
    }

    closedir($handle);
}
?>

<P>Here you see that our directory listing is working perfectly:</p>
<UL>
<P><?=$thelist?></p>
</UL>