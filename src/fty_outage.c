/*  =========================================================================
    fty_agent_outage - Agent outage

    Copyright (C) 2014 - 2015 Eaton                                        
                                                                           
    This program is free software; you can redistribute it and/or modify   
    it under the terms of the GNU General Public License as published by   
    the Free Software Foundation; either version 2 of the License, or      
    (at your option) any later version.                                    
                                                                           
    This program is distributed in the hope that it will be useful,        
    but WITHOUT ANY WARRANTY; without even the implied warranty of         
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
    GNU General Public License for more details.                           
                                                                           
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.            
    =========================================================================
*/

/*
@header
    fty_agent_outage - Agent outage
@discuss
@end
*/

#include "fty_outage_classes.h"

int main (int argc, char *argv [])
{
    bool verbose = false;
    int argn;
    for (argn = 1; argn < argc; argn++) {
        if (streq (argv [argn], "--help")
        ||  streq (argv [argn], "-h")) {
            puts ("fty-agent-outage [options] ...");
            puts ("  --verbose / -v         verbose test output");
            puts ("  --help / -h            this information");
            return 0;
        }
        else
        if (streq (argv [argn], "--verbose")
        ||  streq (argv [argn], "-v"))
            verbose = true;
        else {
            printf ("Unknown option: %s\n", argv [argn]);
            return 1;
        }
    }

    if (getenv ("BIOS_LOG_LEVEL") && streq (getenv ("BIOS_LOG_LEVEL"), "LOG_DEBUG"))
        verbose = true;

    if (verbose)
        zsys_info ("fty_agent_outage - Agent outage");

    zactor_t *server = zactor_new (fty_outage_server, "outage");
    while (true) {
        char *str = zstr_recv (server);
        if (str) {
            puts (str);
            zstr_free (&str);
        }
        else {
            puts ("Interrupted ...");
            break;
        }
    }
    zactor_destroy (&server);
    return 0;
}
