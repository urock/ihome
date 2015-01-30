//struct device_pt parsing_setPort(char *buf){     
//        char port_str[3];  //port
//        char inout_str[4];   // inp/out     
//	char mode_str[2];  // mode input/output
//	char val_str[4];  // value input or output       
//        char def_state_str[4];  // default state 0 or 1 for input or output or ADC or PWM
//        
//        int port;     // int port
//        
//        struct device_pt device_t;
////-----------------------------------------------------------        
//	// Parsing Port
//	strncpy(port_str, &buf[11],2);  //Port number, all port 99
//	port_str[2] = '\0';
//        sscanf(port_str, "%d", &port); // «аписать в device_t.port число, содержащеес€ в строке port_str
//        if (getPort_name(port) != Port100 ){
//          device_t.Port = getPort_name(port);
//          device_t.noPort = 1;
//        }
//        else{
//          device_t.noPort = 0;  // Port is not exist
//          return device_t;
//        }
////-----------------------------
//	//Parsing IN \ OUT
//	strncpy(inout_str,&buf[14],3);  //in or out
//        inout_str[3] = '\0';
////-----------------------------
//	// Mode input
//	  mode_str[0] = buf[17]; //mode input
//	  mode_str[1] = '\0';
//          sscanf(mode_str, "%d", &device_t.mode); // «аписать в j число, содержащеес€ в строке str
//                                                        // Mode input (2- interrupt front, 3- interrupt raise, 4 - interrupt both, 5 - ADC /
//                                                        //Mode output - 1- switch, 2- pwm
////-----------------------------
//		if (strcmp(inout_str,"inp")==0){
//			  // if input
//		 //----  input mode ------------
//                          device_t.direction = INPUT;  //Input
////                        setPort_interrupt(device_t.Port, device_t.mode);
//		}  // end if
//		else if (strcmp(inout_str,"out")==0){
//			// Get out value
//                          device_t.direction = OUTPUT; //Output                          
//			  strncpy(val_str,&buf[19],3);  // value Output;
//			  val_str[3] = '\0';
//			  device_t.value = atoi(val_str);
////                          setPort(device_t.Port, device_t.mode, device_t.value);
//		}  // end else
////--------------------------------
//    strcpy(device_t.port_html, "/portxx.html" );
//
//    sprintf(port_str, "%d", port);
//    if (port < 10){
//      device_t.port_html[5] = '0';
//      device_t.port_html[6] = port_str[0];
//    }
//    else{
//      device_t.port_html[5] = port_str[0];
//      device_t.port_html[6] = port_str[1];
//    }
//  return device_t;
//} // end function
