#pragma once

#include "httpd.h"

#include <map>
#include <string>

using namespace std;

typedef int (* ROUTING_PROC)(request_rec *r, const char *file_path);

extern int entryCreatePuzzle(request_rec *r, const char *file_path);
extern int entryIndexHtml(request_rec *r, const char *file_path);
extern int entryAnyContent(request_rec *r, const char *file_path);

map<string, ROUTING_PROC> Routing = {
	{"/numberplace/create_puzzle.json", entryCreatePuzzle},
	{"/numberplace/", entryIndexHtml},
	{"/numberplace/index.html", entryIndexHtml},
	{"*", entryAnyContent}
};
