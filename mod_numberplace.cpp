/*
**  mod_numberplace.c -- Apache sample numberplace module
**  [Autogenerated via ``apxs -n numberplace -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory
**  by running:
**
**    $ apxs -c -i mod_numberplace.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /numberplace in as follows:
**
**    #   apache2.conf
**    LoadModule numberplace_module modules/mod_numberplace.so
**    <Location /numberplace>
**    SetHandler numberplace
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /numberplace and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/numberplace
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**
**    The sample page from mod_numberplace.c
*/

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

#include "Router.hpp"

#include <stdio.h>
#include <iostream>

using namespace std;

extern "C" module AP_MODULE_DECLARE_DATA numberplace_module;

/* The sample content handler */
static int numberplace_handler(request_rec *r)
{
	if (strcmp(r->handler, "numberplace")) {
		return DECLINED;
	}
#if 0
	r->content_type = "text/html";

	if (!r->header_only)
		ap_rputs("The sample page from mod_numberplace.cpp\n", r);
#else
	if (Routing.count(r->parsed_uri.path) > 0) {
		Routing[r->parsed_uri.path](r, r->parsed_uri.path);
	} else {
		if (Routing["*"](r, r->parsed_uri.path) != OK) {
			r->content_type = "text/html";
			ap_rprintf(r, "Not Found");

			r->status = HTTP_NOT_FOUND;
		}
	}
#endif
	return OK;
}

static void numberplace_register_hooks(apr_pool_t *p)
{
	ap_hook_handler(numberplace_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

extern "C" {
	/* Dispatch list for API hooks */
	module AP_MODULE_DECLARE_DATA numberplace_module = {
		STANDARD20_MODULE_STUFF,
		NULL,				/* create per-dir    config structures */
		NULL,				/* merge  per-dir    config structures */
		NULL,				/* create per-server config structures */
		NULL,				/* merge  per-server config structures */
		NULL,				/* table of config file commands       */
		numberplace_register_hooks	/* register hooks                      */
	};
};
