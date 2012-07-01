#include "main.h"




timekeeper::timeKeeper::timeKeeper() : started(false), startStop("Start timer"), timeLabel("00:00:00") {
	set_title("Timekeeper");
	set_default_size(400, 300);

	int count = 0;	

	//Set up date drop down combo box
	comboModel = Gtk::ListStore::create(timekeeper::comboColumns);
	dateDropDown.set_model(comboModel);

	Gtk::TreeModel::Row row;

	//TODO: Create a loop to add all the items to the drop down
	row = * (comboModel->append());
	row[timekeeper::comboColumns.id] = count++;
	row[timekeeper::comboColumns.datestring] = "Test";
	dateDropDown.set_active(row);

	row = * (comboModel->append());
	row[timekeeper::comboColumns.id] = count++;
	row[timekeeper::comboColumns.datestring] = "Test 2";

	dateDropDown.pack_start(timekeeper::comboColumns.datestring);
	dateDropDown.signal_changed().connect(sigc::mem_fun(*this, &timekeeper::timeKeeper::dateChanged));


	//Pack the layouts and display them
	timerLayout.add(startStop);
	timerLayout.add(timeLabel);

	layoutBox.pack_start(dateDropDown);
	//layoutBox.add(displayTimes);
	layoutBox.pack_start(timerLayout);

	dateDropDown.show();
	timerLayout.show();
	startStop.show();
	timeLabel.show();

	this->add(layoutBox);
	layoutBox.show();
}

void timekeeper::timeKeeper::dateChanged(){
	Gtk::TreeModel::iterator active = dateDropDown.get_active();
	std::cout << "selected " << (*active)[timekeeper::comboColumns.id] << "\n";
}



time_t lastStart;
bool done = false;
double dayTotal;
int month;
int day;
int year; 


void displayTimeDone(){
	while(true){
		double timediff = difftime(time(NULL), lastStart);
		timediff += dayTotal;
		int hours = (int) timediff / 3600;
		timediff -= hours * 3600;
		int minutes = (int) timediff / 60;
		timediff -= minutes * 60;

		std::cout << "\r" << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << timediff << std::flush;
		if(done == true){
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main(int argc, char * argv[]){
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "job.timekeeper");
	timekeeper::timeKeeper mainWindow;
	return app->run(mainWindow);


	std::string input;
	
	FILE * saveFile = fopen("timeinformation", "ab+");
	
	std::vector<time_t> timeVector;
	time_t temp;
	

	if(saveFile == NULL){
		std::cerr << "Error opening file " << strerror(errno) << "\n";
		return 1;
	}

	while(true){

		fread(&temp, sizeof(time_t), 1, saveFile);
		if(!feof(saveFile)){
			timeVector.push_back(temp);
		}else{
			break;
		}
	}

	tm * timeInfo, * timeInfo2;
	time_t timing;
	
	if(argc == 2 && strcmp(argv[1], "times") == 0 && timeVector.size() > 0){
		fclose(saveFile);
		int outyear;
		int outday;
		int outmon;
		
		int tothour = 0, totmin = 0, totsec = 0;
		int difference  = 0;
		for(size_t x = 0; x < timeVector.size(); x+=2){
			if(x != 0){
				free(timeInfo);
				free(timeInfo2);
			}


			timeInfo = (tm *) malloc(sizeof(tm));
			timeInfo2 = (tm *) malloc(sizeof(tm));


			localtime_r(&timeVector[x], timeInfo);
			localtime_r(&timeVector[x+1], timeInfo2);


			if((x == 0) || !(outyear == timeInfo->tm_year+1900 && outday == timeInfo->tm_mday && outmon == timeInfo->tm_mon)){
				if(x != 0){
					tothour += difference / 3600;
					difference = difference % 3600;
					totmin += difference / 60;
					difference = difference % 60;
					totsec += difference;
					printf("Total: %02d:%02d:%d\n", tothour, totmin, totsec);
					difference = tothour = totmin = totsec = 0;
				}
				outyear = timeInfo->tm_year + 1900;
				outday = timeInfo->tm_mday;
				outmon = timeInfo->tm_mon;
				printf("Date (MM/DD/YYYY): %02d/%02d/%d\n", outmon, outday, outyear );
			}
			printf("%02d:%02d:%02d - %02d:%02d:%02d\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, timeInfo2->tm_hour, timeInfo2->tm_min, timeInfo2->tm_sec);
			
			difference += (int) difftime(timeVector[x+1], timeVector[x]);


		}
		tothour += difference / 3600;
		difference = difference % 3600;
		totmin += difference / 60;
		difference = difference % 60;
		totsec += difference;
		printf("Total: %02d:%02d:%02d\n", tothour, totmin, totsec);
		
		return 0;
	}
	

	//Get today

	time(&timing);
	timeInfo = localtime(&timing);
	
	year = 1900 + timeInfo->tm_year;
	month = timeInfo->tm_mon;
	day = timeInfo->tm_mday;

	std::cout << "Day today: " << year << " " <<  month << " " << day << std::endl;

	for(size_t i = 0; i < timeVector.size(); i++){
		timeInfo = localtime(&timeVector[i]);
		if(year == 1900 + timeInfo->tm_year && month == timeInfo->tm_mon && day == timeInfo->tm_mday){
			dayTotal += difftime(timeVector[i+1], timeVector[i]);
			i++;
		}
	}

	time(&lastStart);
	std::thread timer(displayTimeDone);
	while(true){
		getline(std::cin, input);
		if(input == "done"){
			done = true;
			timer.join();

			time(&timing);
			fwrite(&lastStart, sizeof(time_t), 1, saveFile);
			fwrite(&timing, sizeof(time_t), 1, saveFile);

			fclose(saveFile);

			std::cout << std::endl;
			return 0;
		}
	}
	return 0;
}
