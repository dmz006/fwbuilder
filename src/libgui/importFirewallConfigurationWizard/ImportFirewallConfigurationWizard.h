/*

                          Firewall Builder

                 Copyright (C) 2011 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@fwbuilder.org

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

#ifndef __IMPORTFIREWALLCONFIGURATIONWIZARD_H_
#define __IMPORTFIREWALLCONFIGURATIONWIZARD_H_

#include <QWizard>
#include <QStringList>

namespace libfwbuilder
{
    class Firewall;
};


class ImportFirewallConfigurationWizard : public QWizard
{
    Q_OBJECT;
    QString platform;
    QStringList buffer;
    libfwbuilder::Firewall *fw;
    
public:
    enum { Page_FileName, Page_Platform, Page_FirewallName,
           Page_Progess, Page_NetworkZones };
    
    ImportFirewallConfigurationWizard(QWidget *parent);
    virtual ~ImportFirewallConfigurationWizard() {}

    QString getPlatform() { return platform; }
    void setPlatform(const QString &s) { platform = s; }

    QStringList* getBufferPtr() { return &buffer; }

    libfwbuilder::Firewall* getFirewall() { return fw; }
    void setFirewall(libfwbuilder::Firewall* _fw) { fw = _fw; }
    
public slots:
    virtual void accept();
};


#endif
