/*
    Copyright (C) 2005 Cockos Incorporated

    Wahjam is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Wahjam is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Wahjam; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  License dialog related code...

  */

#include <windows.h>
#include <commctrl.h>
#include <math.h>

#include "winclient.h"

#include "resource.h"

static BOOL WINAPI LicenseProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_INITDIALOG:
      SetDlgItemText(hwndDlg,IDC_LICENSETEXT,(char *)lParam);
    return 0;
    case WM_CLOSE:
      EndDialog(hwndDlg,0);
    return 0;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDC_CHECK1:
          EnableWindow(GetDlgItem(hwndDlg,IDOK),!!IsDlgButtonChecked(hwndDlg,LOWORD(wParam)));
        break;
        case IDOK:
          EndDialog(hwndDlg,1);
        break;
        case IDCANCEL:
          EndDialog(hwndDlg,0);
        break;
      }
    return 0;

  }
  return 0;
}

static char *g_need_license;
static int g_license_result;

int licensecallback(int user32, char *licensetext)
{
  if (!licensetext || !*licensetext) return 1;

  g_need_license=licensetext;
  g_license_result=0;
  g_client_mutex.Leave();
  while (g_need_license && !g_done) Sleep(100);
  g_client_mutex.Enter();
  return g_license_result;
}



void licenseRun(HWND hwndDlg)
{
  if (g_need_license)
  {
    g_license_result=DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_LICENSE),hwndDlg,LicenseProc,(LPARAM)g_need_license);
    g_need_license=0;
  }
}