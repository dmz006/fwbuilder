/* 

                          Firewall Builder

                 Copyright (C) 2009 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@vk.crocodile.org

  $Id: CompilerDriver.cpp 1533 2009-10-01 16:42:02Z vadim $

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "../../config.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#ifndef _WIN32
#  include <unistd.h>
#  include <pwd.h>
#else
#  include <direct.h>
#  include <stdlib.h>
#  include <io.h>
#endif

#include "CompilerDriver.h"
#include "Configlet.h"

#include "fwbuilder/FWObject.h"
#include "fwbuilder/Cluster.h"
#include "fwbuilder/Firewall.h"
#include "fwbuilder/Resources.h"

#include "fwcompiler/OSConfigurator.h"

#include <QStringList>
#include <QFileInfo>


using namespace std;
using namespace libfwbuilder;
using namespace fwcompiler;



QString CompilerDriver::printPathForAllTools(Firewall*, const std::string &)
{
    return "";
}

QString CompilerDriver::printActivationCommands(Firewall*)
{
    return "";
}

QString CompilerDriver::assembleManifest(Firewall*)
{
    return "";
}

void CompilerDriver::assembleFwScriptInternal(Firewall* fw,
                                              OSConfigurator *oscnf,
                                              Configlet *script_skeleton,
                                              Configlet *top_comment)
{
    FWOptions* options = fw->getOptionsObject();
    string platform = fw->getStr("platform");
    string fw_version = fw->getStr("version");
    string host_os = fw->getStr("host_OS");
    string family = Resources::os_res[host_os]->Resources::getResourceStr(
        "/FWBuilderResources/Target/family");
    bool debug = options->getBool("debug");
    string shell_dbg = (debug)?"set -x":"" ;
    string cmd_dbg = (debug)?"-v ":"";
    string prolog_place = options->getStr("prolog_place");
    if (prolog_place.empty()) prolog_place = "fw_file";  // old default
    string pre_hook = fw->getOptionsObject()->getStr("prolog_script");
    string firewall_dir = options->getStr("firewall_dir");
    if (firewall_dir=="") firewall_dir = "/etc/fw";

    char *timestr;
    time_t tm;
    struct tm *stm;

    tm = time(NULL);
    stm = localtime(&tm);
    timestr = strdup(ctime(&tm));
    timestr[strlen(timestr)-1] = '\0';
    
#ifdef _WIN32
    char* user_name=getenv("USERNAME");
#else
    struct passwd *pwd=getpwuid(getuid());
    assert(pwd);
    char *user_name=pwd->pw_name;
#endif
    if (user_name==NULL)
    {
        user_name=getenv("LOGNAME");
        if (user_name==NULL)
            abort("Can't figure out your user name");
    }

    QString script_buffer;
    QTextStream script(&script_buffer, QIODevice::WriteOnly);

    script_skeleton->removeComments();
    script_skeleton->setVariable("shell_debug", shell_dbg.c_str());
    script_skeleton->setVariable("firewall_dir", firewall_dir.c_str());

    top_comment->setVariable("version", VERSION);
    QString build_num;
    build_num.setNum(BUILD_NUM);
    top_comment->setVariable("build", build_num);
 
    top_comment->setVariable("timestamp", timestr);
    top_comment->setVariable("tz", tzname[stm->tm_isdst]);
    top_comment->setVariable("user", user_name);

    QFileInfo fw_file_info(fw_file_name);

    top_comment->setVariable("manifest", assembleManifest(fw));
    top_comment->setVariable("platform", platform.c_str());
    top_comment->setVariable("fw_version", fw_version.c_str());
    top_comment->setVariable("comment", prepend("# ", fw->getComment().c_str()));

    script_skeleton->setVariable("have_nat", have_nat);
    script_skeleton->setVariable("have_filter", have_filter);

    script_skeleton->setVariable("top_comment", top_comment->expand());
    script_skeleton->setVariable("errors_and_warnings",
                                prepend("# ", all_errors.join("\n")));
    script_skeleton->setVariable("tools", printPathForAllTools(fw, family));

    script_skeleton->setVariable("timestamp", timestr);
    script_skeleton->setVariable("user", user_name);
    if (prolog_place == "fw_file")
        script_skeleton->setVariable("prolog_script", pre_hook.c_str());
    else
        script_skeleton->setVariable("prolog_script", "");

    script_buffer = "";

    script_skeleton->setVariable("shell_functions", oscnf->printFunctions().c_str());
    script_skeleton->setVariable("kernel_vars_commands",
                                prepend("    ", oscnf->printKernelVarsCommands().c_str()));
    script_skeleton->setVariable("configure_interfaces",
                                prepend("    ", oscnf->configureInterfaces().c_str()));

    // this really adds nothing for the most of the systems
    script_skeleton->setVariable("other_os_configuration_commands", oscnf->getCompiledScript().c_str());

    script_skeleton->setVariable("activation_commands", printActivationCommands(fw));

    script_skeleton->setVariable("verify_interfaces", "");

    script_skeleton->setVariable("epilog_script",
                                fw->getOptionsObject()->getStr("epilog_script").c_str());
}
