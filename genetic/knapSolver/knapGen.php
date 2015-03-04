<?php
//knapsack generator
echo("enter the minimum possible cost\n");
fscanf(STDIN, "%d\n", $minCost);
echo("enter the maximum possible cost\n");
fscanf(STDIN, "%d\n", $maxCost);
echo("enter the minimum possible value\n");
fscanf(STDIN, "%d\n", $minVal);
echo("enter the maximum possible value\n");
fscanf(STDIN, "%d\n", $maxVal);
echo("enter the number of items in the sack\n");
fscanf(STDIN, "%d\n", $numItems);
$fileOut = fopen("knapout.txt", "wb");
//make a good costlimit
$costLimit=$numItems*$minCost;
$costLimit.=PHP_EOL;
fwrite($fileOut, $costLimit);
for($i=0;$i<$numItems;$i++){
	$array=array();
	$temp = $i;
	$name="";
	while($temp>=0){
		$whichChar=65 + $temp%26;
		array_unshift($array,$whichChar);
		$temp=floor($temp/26);
		$temp-=1;
	}
	foreach($array as $c){
		$name.=chr($c);
	}
	$myCost=rand($minCost, $maxCost);
	$myVal = rand($minVal, $maxVal);
	$nextLine= $name.",".$myCost.",".$myVal.PHP_EOL;
	fwrite($fileOut, $nextLine);
}
$bool=fclose($fileOut);
if(!$bool){
	echo ("EoF may not have written\n");
}
echo("Script complete\n");
?>