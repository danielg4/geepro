/* $Revision: 1.5 $ */
/* geepro - Willem eprom programmer for linux
 * Copyright (C) 2006 Krzysztof Komarnicki
 * Email: krzkomar@wp.pl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. See the file COPYING. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

extern "C" {
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <signal.h>

#include "buffer.h"
#include "../gui/gui.h"
#include "main.h"
#include "chip.h"
#include "../intl/lang.h"
#include "../drivers/hwplugin.h"
#include "dummy.h"
#include "geepro.h"
#include "storings.h"


/* uchwyt api do wybranego sterownika, global na cały program */
hw_module_type ___hardware_module___ = dummy_hardware_module; 

/*global do zmiennych przechowywanych w pliku */
store_str store;
}

//#include <string>

/* globalna zmienna zawierająca uzytkownika */
int ___uid___= -1;

int test_uid(geepro *gep)
{
    if((___uid___ = getuid())) 
	gui_dialog_box(gep, "[WN][TEXT]\n   For lower latency time \n please run program using \n                 sudo[/TEXT][BR]  OK  ");
    return ___uid___;
}

char test_hw(void *wg, geepro *gep)
{
    char ex;

//    for(;;)
	if(hw_test_conn()){
//    	    if(stop)
        	gui_dialog_box(gep,"[IF][TEXT]\n     Hardware present [/TEXT][BR]  OK  ");
    	    ex = 0;		    	    
//	    break;
	}else{
//	    if(stop)
//    		gui_dialog_box(gep, "Error !!!","\n hardware error :  Check Power and\n connections");
//	    else
		if(gui_dialog_box(gep,"[ER][TEXT]\n hardware error :  Check Power and\n connections[/TEXT][BR]Try again[BR]Cancel") == 2) return 0;
    	    ex = 1;	
//	    if(stop) break;
	}
    return 1;
}

/*
  conf.define('DEFAULT_PLUGINS_PATH'       , conf.env.PREFIX+'/lib/geepro/plugins')
  conf.define('DEFAULT_DRIVERS_PATH'       , conf.env.PREFIX+'/lib/geepro/drivers')
  conf.define('DEFAULT_SHARE_DRIVERS_PATH', conf.env.PREFIX+'/share/geepro/drivers')
*/
/*
bool find_directory_of_file(string &dir,const string file,const vector<string>lookthere)
{
    for (vector<string>::iterator s = codeTable.begin(); s != lookthere.end(); s++) {
        cout << s <<endl;
}
      
*/

int main(int argc, char **argv)
{
    geepro geep;
    gui g;

    geep.gui = &g;
    geep.ifc = iface_init();
    geep.ifc->gep = &geep;
    geep.argc = argc;
    geep.argv = argv;
    geep.chp = NULL;
/*
    // Looking for prefix location of data files
    char plugin_path[256];
    find_directory_of_file(plugin_path,"libwillem.so",[DEFAULT_PLUGINS_PATH,"./drivers","/usr/lib/geepro/plugins"],3);
*/
    store_constr(&store, "~/.geepro","geepro.st");
// do poprawki jak będzie config - te wszystkie stałe mają być pobierane z pliku configuracyjnego 
    iface_plugin_allow(geep.ifc, "willem:dummy:jtag");
    iface_module_allow(geep.ifc, "prom:mcs51:mcs48:exampl:93Cxx:27xx:24Cxx:28xx");
    iface_load_config(geep.ifc, NULL);
    iface_make_plugin_list(geep.ifc, "./drivers", ".plug");
    gui_menu_setup(&geep);
/* moduły chipów inicjują menu gui, dlatego gui musi być zainicjowane */
/* parametry z configa w przyszłości */
    iface_make_modules_list( geep.ifc, "./plugins", ".module"); 

//    signal(SIGINT, kill_me);
    
    gui_run(&geep);

    iface_destroy(geep.ifc);
    store_destr(&store);
    return 0;
}

