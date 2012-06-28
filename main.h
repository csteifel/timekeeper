#include <iostream>
#include <time.h>
#include <thread>
#include <string>
#include <chrono>
#include <iomanip> 
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gtkmm.h>


namespace timekeeper {
	
	class timeKeeper : public Gtk::Window {
		public:
			timeKeeper() : started(false), startStop("Start timer"), timeLabel("00:00:00") {
					timerLayout.add(startStop);
					timerLayout.add(timeLabel);
				}
			~timeKeeper(){}
		private:
			//void buttonPressed();
			bool started;
			Gtk::Button startStop;
			Gtk::Label timeLabel;
			Gtk::VBox layoutBox;
			Gtk::HBox timerLayout;
	};


}
