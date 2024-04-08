<?php
	parse_str($_SERVER['QUERY_STRING'], $query);
    $a = $query['a'];
	$b = $query['b'];
    $result = $a + $b;

	echo "Content-type: text/html\n\n";
    echo "<html>";
    echo "<head>";
    echo "<title>Hello - Second CGI Program</title>";
    echo "</head>";
    echo "<body>";
    echo "<h2>" . $a . " + " . $b . " = " . $result . "</h2>";
    echo "</body>";
    echo "</html>";
?>