#include <apr_uri.h>
#include <apr_fnmatch.h>
#include <unistd.h>
#include "commands.h"
#include "bstrlib.h"
#include "db.h"
#include "shell.h"
#include "dbg.h"

int Command_depends(apr_pool_t* p, const char* path)
{
	FILE* in = NULL;
	bstring line = NULL;
	in = fopen(path, "r");
	check(in != NULL, "Failed to open downloaded depends: %s", path);
	for(line = bgets((bNgetc)fgetc, in, '\n');
			line != NULL;
		line = bgets((bNgetc)fgetc, in, '\n'))
	{
		btrimws(line);
		log_info("Processing depends: %s", bdata(line));
		int rc = Command_install(p, bdata(line), NULL, NULL, NULL);
		check(rc == 0, "Failed to install: %s", bdata(line));
		bdestroy(line);
	}
	fclose(in);
	return 0;
error:
	if(line) bdestroy(line);
	if(in) fclose(in);
	return -1;
}

int Command_fetch(apr_pool_t* p, const char* url, int feth_only)
{
	apr_uri_t info = {.port = 0};
	int rc = 0;
	const char* depends_file = NULL;

	apr_status_t rv = apr_uri_parse(p, url, &info);
	check(rv == APR_SUCCESS, "Failed to parse URL: %s", url);

	if(apr_fnmatch(GIT_PAT, info.path, 0) == APR_SUCCESS) {
		rc = Shell_exec(GIT_SH, "URL", url, NULL);
		check(rc == 0, "Git failed.");
	} else if(apr_fnmatch(DEPEND_PAT, info.path, 0) == APR_SUCCESS) {
		check(!fetch_only, "No point in fetching a DEPENDS file.");
		if(info.scheme) {
			depends_file = DEPENDS_PATH;
			rc = Shell_exec(CURL_SH, "URL", url,
					"TARGET", depends_file, NULL);
			check(rc == 0, "Curl failed.");
		} else {
			depends_file = info.path;
		}

		// Recursively process the devpkg list:
		log_info("Building according to DEPENDS: %s", url);
		rv = Command_depends(p, depends_file);
		check(rv == 0, "Failed to process the DEPENDS: %s", url);
		// When that returns with a 0, it means nothing else
		// needs to be done, so we return:
		return 0;
	} else if(apr_fnmatch(TAR_GZ_PAT, info.path, 0) == APR_SUCCESS) {


