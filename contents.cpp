#include "httpd.h"
#include "http_protocol.h"

#include "apr_file_io.h"	// for probing file

#include "NumberPlace.hpp"

#include <string>
#include <map>

static const std::map<std::string, std::string> tableContentType = {
	{".css",	"text/css"			},
	{".html",	"text/html"			},
	{".htm",	"text/html"			},
	{".js",		"text/javascript"	},
	{".json",	"application/json"	},
};

void PrintJson(NumberPlace &p_numpla, request_rec *r)
{
	ap_rputs("{", r);
	ap_rputs("\"List\":[", r);
	([&]() {
		size_t l_counter = 0;

		for (auto l_value : p_numpla.GetCellList()) {
			ap_rprintf(r, "%u", l_value);
			if (l_counter != p_numpla.GetCellList().size() - 1) {
				ap_rputs(",", r);
			}
			l_counter++;
		}
	})();
	ap_rputs("],", r);
	ap_rputs("\"Answer\":[", r);
	([&]() {
		size_t l_counter = 0;

		for (auto l_value : p_numpla.GetAnswerCellList()) {
			ap_rprintf(r, "%u", l_value);
			if (l_counter != p_numpla.GetAnswerCellList().size() - 1) {
				ap_rputs(",", r);
			}
			l_counter++;
		}
	})();
	ap_rputs("]", r);
	ap_rputs("}", r);
}

std::string getContentType(const std::string &file_path)
{
	auto extension = ([&]() {
		for (size_t idx = file_path.size(); idx > 0; idx--) {
			if (file_path.c_str()[idx] == '.') {
				return string(&file_path.c_str()[idx]);
			}
		}
		
		return string("");
	})();

	return tableContentType.at(extension.size() > 0 ? extension : ".html");
}

int print_contents(request_rec *r, const char *file_path)
{
	apr_status_t result = APR_SUCCESS;
	apr_file_t *file = nullptr;
	char path[256];

	sprintf(path, "/var/www/module/%s", file_path);

	result = apr_file_open(&file, path,
			APR_FOPEN_READ | APR_FOPEN_BINARY, APR_OS_DEFAULT, r->pool);
	if (result != APR_SUCCESS) {
		ap_rputs("<html>", r);
		ap_rputs("<body>", r);
		ap_rputs("<h1>Internal Server Error</h1>", r);
		ap_rputs("<hr>", r);
		ap_rputs("apr_file_open function is failed.", r);
		ap_rputs("</body>", r);
		ap_rputs("</html>", r);
		r->status = HTTP_INTERNAL_SERVER_ERROR;
		return OK;
	}

	do {
		char buff[4096];
		apr_size_t size = sizeof(buff);

		result = apr_file_read(file, buff, &size);
		if (result != APR_SUCCESS && result != APR_EOF) {
			ap_rputs("<html>", r);
			ap_rputs("<body>", r);
			ap_rputs("<h1>Internal Server Error</h1>", r);
			ap_rputs("<hr>", r);
			ap_rputs("apr_file_read function is failed.", r);
			ap_rputs("</body>", r);
			ap_rputs("</html>", r);
			r->status = HTTP_INTERNAL_SERVER_ERROR;
			return OK;
		}

		ap_rwrite(buff, static_cast<int>(size), r);
	} while(result != APR_EOF);

	result = apr_file_close(file);
	if (result != APR_SUCCESS) {
		ap_rputs("<html>", r);
		ap_rputs("<body>", r);
		ap_rputs("<h1>Internal Server Error</h1>", r);
		ap_rputs("<hr>", r);
		ap_rputs("apr_file_close function is failed.", r);
		ap_rputs("</body>", r);
		ap_rputs("</html>", r);
		r->status = HTTP_INTERNAL_SERVER_ERROR;
		return OK;
	}

	return OK;
}

int entryCreatePuzzle(request_rec *r, const char *file_path)
{
	r->content_type = "application/json";

	if (!r->header_only) {
		NumberPlace l_numpla;

		l_numpla.Create();
		PrintJson(l_numpla, r);
	}

	return OK;
}

int entryIndexHtml(request_rec *r, const char *file_path)
{
	r->content_type = "text/html";

	if (!r->header_only) {
		print_contents(r, "/numberplace/index.html");
	}

	return OK;
}

int entryAnyContent(request_rec *r, const char *file_path)
{
	string content_type = getContentType(file_path);
	char *buff = reinterpret_cast<char*>(apr_palloc(r->pool, content_type.size() + 1));

	r->content_type = buff;
	strcpy(buff, content_type.c_str());
	buff[strlen(buff)] = '\0';

	if (!r->header_only) {
		print_contents(r, file_path);
	}

	return OK;
}
