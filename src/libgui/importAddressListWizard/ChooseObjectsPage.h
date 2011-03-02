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

#ifndef __CHOOSEOBJECTSPAGE_H_
#define __CHOOSEOBJECTSPAGE_H_

#include "../../config.h"
#include "ui_chooseobjectspage_q.h"

#include "fwbuilder/InetAddr.h"

#include "FilterDialog.h"

#include <QStringList>


class ChooseObjectsPage : public QWizardPage
{
    Q_OBJECT;

    Ui::ChooseObjectsPage_q *m_dialog;

    Filter * flt_obj;
    FilterDialog * flt_obj_d;
    std::map<libfwbuilder::InetAddr, QStringList> reverse_hosts;
    QStringList objects_to_use;

    // configure this as a proprty so it can be accessed as a field after
    // registering with registerField(). Now it can be accessed from
    // other pages of the wizard
    Q_PROPERTY(QStringList objectsToUse READ getObjectsToUse WRITE setObjectsToUse);
    
public:
    ChooseObjectsPage(QWidget *parent);
    virtual ~ChooseObjectsPage() {}

    virtual void initializePage();
    virtual bool validatePage();

    virtual bool isComplete() const;

    void fillListOfObjects();
    void updateObjectsToUse();

    QStringList getObjectsToUse() { return objects_to_use; }
    void setObjectsToUse(const QStringList &lst) { objects_to_use = lst; }
    
public slots:

    void addFilter();
    void removeFilter();
    void selectAllResults();
    void unselectAllResults();
    void selectAllUsed();
    void unselectAllUsed();
    void addObject();
    void removeObject();
    
};


#endif
