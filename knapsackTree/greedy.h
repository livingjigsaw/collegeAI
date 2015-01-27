
using namespace std;

void sortCost(vector<knapItem*>& costsort, int leftBound, int rightBound){ //recursive, call with 0 and size - 1 the first time
	//NOTE: sorts LOW to HIGH
	if(leftBound>=rightBound){
		return; // single length array, trivially sorted
	}

	int wall=leftBound; //this is the first element to the right of pivot
	int pivot=rightBound; //pick rightmost as pivot
	int current=leftBound;
	knapItem* temp; //for swapping
	while(current!=pivot){
		if(costsort[current]->cost < costsort[pivot]->cost){
			//swap wall and current values
			temp=costsort[wall];
			costsort[wall]=costsort[current];
			costsort[current]=temp;
			//wall moves right
			wall++;
		}
		current++;
	}
	//now swap pivot and wall
	temp=costsort[wall];
	costsort[wall]=costsort[pivot];
	costsort[pivot]=temp;
	//val at wall is now pivot and in the correct place
	//recursively call on subarrays
	sortCost(costsort, leftBound, wall-1);
	sortCost(costsort, wall+1, rightBound);
}

void sortValue(vector<knapItem*>& valuesort, int leftBound, int rightBound){ //recursive, call with 0 and size - 1 the first time
	//NOTE: sorts HIGH to LOW
	if(leftBound>=rightBound){
		return; // single length array, trivially sorted
	}

	int wall=leftBound; //this is the first element to the right of pivot
	int pivot=rightBound; //pick rightmost as pivot
	int current=leftBound;
	knapItem* temp; //for swapping
	while(current!=pivot){
		if(valuesort[current]->value > valuesort[pivot]->value){
			//swap wall and current values
			temp=valuesort[wall];
			valuesort[wall]=valuesort[current];
			valuesort[current]=temp;
			//wall moves right
			wall++;
		}
		current++;
	}
	//now swap pivot and wall
	temp=valuesort[wall];
	valuesort[wall]=valuesort[pivot];
	valuesort[pivot]=temp;
	//val at wall is now pivot and in the correct place
	//recursively call on subarrays
	sortValue(valuesort, leftBound, wall-1);
	sortValue(valuesort, wall+1, rightBound);
}

void sortRatio(vector<knapItem*>& ratiosort, int leftBound, int rightBound){ //recursive, call with 0 and size - 1 the first time
	//NOTE: sorts HIGH to LOW
	if(leftBound>=rightBound){
		return; // single length array, trivially sorted
	}

	int wall=leftBound; //this is the first element to the right of pivot
	int pivot=rightBound; //pick rightmost as pivot
	int current=leftBound;
	knapItem* temp; //for swapping
	while(current!=pivot){
		if(ratiosort[current]->ratio > ratiosort[pivot]->ratio){
			//swap wall and current ratios
			temp=ratiosort[wall];
			ratiosort[wall]=ratiosort[current];
			ratiosort[current]=temp;
			//wall moves right
			wall++;
		}
		current++;
	}
	//now swap pivot and wall
	temp=ratiosort[wall];
	ratiosort[wall]=ratiosort[pivot];
	ratiosort[pivot]=temp;
	//val at wall is now pivot and in the correct place
	//recursively call on subarrays
	sortRatio(ratiosort, leftBound, wall-1);
	sortRatio(ratiosort, wall+1, rightBound);
}

void greedyAnswers(vector<knapItem*> original, int costLimit, gAnswers& gAns){
	vector<knapItem*> costList = original;
	vector<knapItem*> valList = original;
	vector<knapItem*> ratList = original; //to be sorted variations of the original list

	//cout << "now sorting\n";

	sortCost(costList, 0, costList.size()-1);
	sortValue(valList, 0, valList.size()-1);
	sortRatio(ratList, 0, ratList.size()-1);

	gAns.cost.reset();
	//cost greedy
	for(int i=0;i<costList.size();i++){
		if(gAns.cost.totalCost+costList[i]->cost <=costLimit){
			gAns.cost.totalCost+=costList[i]->cost;
			gAns.cost.totalValue+=costList[i]->value;
			gAns.cost.itemNames.insert(costList[i]->name);
		}
	}
	// clear solution
	gAns.value.reset();
	//value greedy
	for(int i=0;i<valList.size();i++){
		if(gAns.value.totalCost+valList[i]->cost <=costLimit){
			gAns.value.totalCost+=valList[i]->cost;
			gAns.value.totalValue+=valList[i]->value;
			gAns.value.itemNames.insert(valList[i]->name);
		}
	}
	// clear solution
	gAns.ratio.reset();
	//ratio greedy
	for(int i=0;i<ratList.size();i++){
		if(gAns.ratio.totalCost+ratList[i]->cost <=costLimit){
			gAns.ratio.totalCost+=ratList[i]->cost;
			gAns.ratio.totalValue+=ratList[i]->value;
			gAns.ratio.itemNames.insert(ratList[i]->name);
		}
	}

	// clear solution
	gAns.partial.reset();
	//partial knapsack
	for(int i=0;i<ratList.size();i++){
		if(gAns.partial.totalCost==costLimit){
			break;
		}
		else if(gAns.partial.totalCost+ratList[i]->cost <= costLimit){
			gAns.partial.totalCost+=ratList[i]->cost;
			gAns.partial.totalValue+=ratList[i]->value;
			gAns.partial.itemNames.insert(ratList[i]->name);
		}
		else if(gAns.partial.totalCost+ratList[i]->cost > costLimit){
			double remains = costLimit - gAns.partial.totalCost;
			gAns.partial.totalValue+= remains * (double)ratList[i]->ratio;

			gAns.partial.totalCost+=remains;
			gAns.partial.itemNames.insert(ratList[i]->name);
		}
	}
}