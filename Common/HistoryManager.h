/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: SystemInterface.h,v $
  Language:  C++
  Date:      $Date: 2010/04/14 10:06:23 $
  Version:   $Revision: 1.11 $
  Copyright (c) 2003-2013 Paul A. Yushkevich and Guido Gerig

  This file is part of ITK-SNAP

  ITK-SNAP is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  -----

  Copyright (c) 2003 Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <vector>
#include <map>
#include <string>

class SystemInterface;
class Registry;

/**
 * @brief The HistoryManager class
 * This class manages file IO history in the SNAP application. Starting with
 * version 3.0, there is a global history for each kind of file (main image,
 * segmentation, label descriptions, etc) and a history associated with each
 * main image. This class maintains the history entries.
 */
class HistoryManager
{
public:

  // Typedef for history lists
  typedef std::vector<std::string> HistoryListType;

  /** Update the history in one of the named categories. The last parameter
      is whether the history entry should be made local, i.e., associated
      with the currently loaded main image. */
  void UpdateHistory(const std::string &category,
                     const std::string &file,
                     bool make_local);

  /** Get the local history in a category */
  const HistoryListType &GetLocalHistory(const std::string &category);

  /** Get the global history in a category */
  const HistoryListType &GetGlobalHistory(const std::string &category);

  /** Create a registry holding the local history */
  void SaveLocalHistory(Registry &folder)
    { SaveHistory(folder, m_LocalHistory); }

  /** Read a registry holding the local history */
  void LoadLocalHistory(Registry &folder)
    { LoadHistory(folder, m_LocalHistory); }

  /** Create a registry holding the local history */
  void SaveGlobalHistory(Registry &folder)
    { SaveHistory(folder, m_GlobalHistory); }

  /** Read a registry holding the local history */
  void LoadGlobalHistory(Registry &folder)
    { LoadHistory(folder, m_GlobalHistory); }


  HistoryManager();

protected:

  static const unsigned int HISTORY_SIZE_LOCAL, HISTORY_SIZE_GLOBAL;

  // Array of histories for different types of files
  typedef std::map<std::string, HistoryListType> HistoryMap;
  HistoryMap m_LocalHistory, m_GlobalHistory;

  void SaveHistory(Registry &folder, HistoryMap &hmap);
  void LoadHistory(Registry &folder, HistoryMap &hmap);

  void UpdateHistoryList(
      HistoryListType &hl, const std::string &file, unsigned int maxsize);

  // The system interface class
  SystemInterface *m_SystemInterface;

};

#endif // HISTORYMANAGER_H
