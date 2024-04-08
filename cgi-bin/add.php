<?php
$a = $_POST['a'];
$b = $_POST['b'];
$result = $a + $b;

echo "Content-type:text/html\n\n";
echo "<html>\n";
echo "<head>\n";
echo "<title>Hello - Second CGI Program</title>\n";
echo "</head>\n";
echo "<body>\n";
echo "<h2>$a + $b = $result</h2>\n";
echo "</body>\n";
echo "</html>";
?>
