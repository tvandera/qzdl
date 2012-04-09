/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "ZFileList.h"
#include "ZFileListable.h"
#include "ZDLConfigurationManager.h"

#include <iostream>
using namespace std;

ZFileList::ZFileList(ZQWidget *parent): ZDLListWidget(parent){
	LOGDATAO() << "ZFileList" << endl;
}

void ZFileList::newDrop(QList<QUrl> urlList){
	LOGDATAO() << "newDrop" << endl;	
	for (int i = 0; i < urlList.size() && i < 32; ++i) {
		QUrl url = urlList.at(i);
		LOGDATAO() << "url " << i << "=" << url.toString() << endl;
		if(url.scheme() == "file"){
			QString path = url.path();
#ifdef Q_WS_WIN	
			if(path[i] == '/'){
				path.remove(0,1);
			}	
#endif
			QFileInfo urlDecoder(path);
			LOGDATAO() << "Adding path " << urlDecoder.absoluteFilePath() << endl;
			ZFileListable *zList = new ZFileListable(pList, 1001, urlDecoder.absoluteFilePath());
			insert(zList, -1);
		}
	}
}

void ZFileList::newConfig(){
	LOGDATAO() << "Reading new config" << endl;
	pList->clear();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		QVector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+$", vctr);
		//cout << "I got " << vctr.size() << " matches!" << endl;
		for(int i = 0; i < vctr.size(); i++){
			ZFileListable *zList = new ZFileListable(pList, 1001, vctr[i]->getValue());
			insert(zList, -1);
		}
	}
}

void ZFileList::rebuild(){
	LOGDATAO() << "Saving config" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		QVector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+$", vctr);
		//cout << "I got " << vctr.size() << " matches!" << endl;
		for(int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	//cout << "Building lines" << endl;
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZFileListable* fitm = (ZFileListable*)itm;
		QString name = QString("file").append(QString::number(i));
		zconf->setValue("zdl.save", name, fitm->getFile());
	
	}
	
}

void ZFileList::addButton(){
	LOGDATAO() << "Adding new file" << endl;
	QStringList filters;
	filters << "WAD/PK3/ZIP/PK7/PKZ/P7Z (*.wad *.pk3 *.zip *.pk7 *.pkz *.p7z)"
         << "WAD Files (*.wad)"
         << "PK3 Files (*.pk3)"
	 << "PK7 Files (*.pk7)"
	 << "PKZ Files (*.pkz)"
	 << "P7Z Files (*.pk7)"
         << "zip Files (*.zip)"
         << "Any files (*)";
	QFileDialog dialog(this);
	dialog.setFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			LOGDATAO() << "Adding file " << fileNames[i] << endl;
			ZFileListable *zList = new ZFileListable(pList, 1001, fileNames[i]);
			insert(zList, -1);
		}

	}


	


}

