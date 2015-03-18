#knapsack generator
print "enter the minimum possible cost\n"
minCost =gets.to_i
print "enter the maximum possible cost\n"
maxCost =gets.to_i
print "enter the minimum possible value\n"
minVal =gets.to_i
print "enter the maximum possible value\n"
maxVal =gets.to_i
print "enter the number of items in the sack\n"
numItems =gets.to_i
fileOut = File.new("rubyKnapOut.csv", "w")
#make a good costlimit
costLimit=(numItems * ((minCost+maxCost)/4+minCost)).floor
fileOut << costLimit << "\r\n"
for i in 0..(numItems-1)
	array=Array.new
	temp = i
	name=""
	while temp>=0 do
		whichChar=65 + temp%26
		array.unshift(whichChar)
		temp=(temp/26).floor
		temp-=1
	end
	array.each do |c|
		name+=c.chr
	end
	myCost=rand(minCost..maxCost)
	myVal = rand(minVal..maxVal)
	nextLine= name+","+myCost.to_s+","+myVal.to_s+"\r\n"
	fileOut<<nextLine
end
fileOut.close
print "Script complete\n"
