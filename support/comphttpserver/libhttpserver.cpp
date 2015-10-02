/*
 * libhttpserver.cpp
 *
 *  Created on: 2015. 9. 29.
 *      Author: hwang
 */

#include "libhttpserver.hpp"
#include <string>
#include <iostream>

using namespace std;

int libhttpserver::request_handle (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  string page = "<html><body>Hello, browser!</body></html>";
  struct MHD_Response *response;
  int ret;

  response = MHD_create_response_from_buffer (page.size(), (void *)(page.c_str()), MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}


libhttpserver::libhttpserver(int port):_port(port) {


}

libhttpserver::~libhttpserver() {

}

void libhttpserver::listen(unsigned int port)
{
	/*std::string page = "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo<br>\n<form name=\"Show\">\n<input type=\"text\" name=\"MouseX\" value=\"0\" size=\"4\"> X<br>\n<input type=\"text\" name=\"MouseY\" value=\"0\" size=\"4\"> Y<br>\n</form>\n\n<script language=\"JavaScript1.2\">\n<!--\n\n// Detect if the browser is IE or not.\n// If it is not IE, we assume that the browser is NS.\nvar IE = document.all?true:false\n\n// If NS -- that is, !IE -- then set up for mouse capture\nif (!IE) document.captureEvents(Event.MOUSEMOVE)\n\n// Set-up to use getMouseXY function onMouseMove\ndocument.onmousemove = getMouseXY;\n\n// Temporary variables to hold mouse x-y pos.s\nvar tempX = 0\nvar tempY = 0\n\n// Main function to retrieve mouse x-y pos.s\n\nfunction getMouseXY(e) {\n  if (IE) { // grab the x-y pos.s if browser is IE\n    tempX = event.clientX + document.body.scrollLeft\n    tempY = event.clientY + document.body.scrollTop\n  } else {  // grab the x-y pos.s if browser is NS\n    tempX = e.pageX\n    tempY = e.pageY\n  }  \n  // catch possible negative values in NS4\n  if (tempX < 0){tempX = 0}\n  if (tempY < 0){tempY = 0}  \n  // show the position values in the form named Show\n  // in the text fields named MouseX and MouseY\n  document.Show.MouseX.value = tempX\n  document.Show.MouseY.value = tempY\n  return true\n}\n\n//-->\n</script>\n<TABLE BORDER>\n<CAPTION>légende en haut</CAPTION>\n<TR><TD><IMG src=\"toto.bmp\"></TD></TR>\n<TR><TD>deuxième ligne</TD></TR>\n</TABLE>\n</body></html>";
	struct MHD_Daemon *d;
	d = MHD_start_daemon (// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
	                        MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
	                        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
	                        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
	                        DEFAULT_PORT,
	                        NULL, NULL, &ahc_echo, (void*)page.c_str(),
	                        MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
	                        MHD_OPTION_END);*/
}

void libhttpserver::run()
{
	daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, _port, NULL, NULL, &libhttpserver::request_handle, this, MHD_OPTION_END);
	if(!daemon)
		return;

	MHD_stop_daemon(daemon);
}


