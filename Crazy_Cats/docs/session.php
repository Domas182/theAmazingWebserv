<?php
session_start();
echo "<html>";
echo "<head>";
echo "<title>session.php</title>";
echo "</head>";
echo "<body>";
echo "<h1>session.php</h1>";

// if (isset($username) && isset($password)) {
	echo $_REQUEST;
	echo $_SESSION;
	echo $_COOKIE;
	echo "\nHALO";
	if (isset($_REQUEST['username']) && isset($_REQUEST['password'])) {
	echo "HALLO";
    $username = $_SESSION['username'];
	$password = $_SESSION['password'];
	if (empty($username) || empty($password)) {
        echo "first";
		header("Location: login.php");
	} else {
        echo "second";
		$_SESSION['username'] = $username;
		$_SESSION['password'] = $password;
		echo "Logged in as '$username'. <br>
        Click <a href='page.php'>here</a> to continue.";
		echo "<h4><a href='logout.php'>Logout</a></h4>";
	}
// } else {
// 	$URL="./login.php";
// 	echo "second";
// 	echo "<script type='text/javascript'>document.location.href='{$URL}';</script>";
// 	echo '<META HTTP-EQUIV="refresh" content="0;URL=' . $URL . '">';
}

echo "</body>";
echo "</html>";

?>

<?php
