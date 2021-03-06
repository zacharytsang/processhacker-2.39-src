/*
 * Process Hacker -
 *   Process properties: Token page
 *
 * Copyright (C) 2009-2016 wj32
 *
 * This file is part of Process Hacker.
 *
 * Process Hacker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process Hacker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Process Hacker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <phapp.h>
#include <procprpp.h>

NTSTATUS NTAPI PhpOpenProcessTokenForPage(
    _Out_ PHANDLE Handle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ PVOID Context
    )
{
    NTSTATUS status;
    HANDLE processHandle;

    if (!NT_SUCCESS(status = PhOpenProcess(
        &processHandle,
        ProcessQueryAccess,
        (HANDLE)Context
        )))
        return status;

    status = PhOpenProcessToken(processHandle, DesiredAccess, Handle);
    NtClose(processHandle);

    return status;
}

INT_PTR CALLBACK PhpProcessTokenHookProc(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
    )
{
    switch (uMsg)
    {
    case WM_DESTROY:
        {
            RemoveProp(hwndDlg, PhMakeContextAtom());
        }
        break;
    case WM_SHOWWINDOW:
        {
            if (!GetProp(hwndDlg, PhMakeContextAtom())) // LayoutInitialized
            {
                PPH_LAYOUT_ITEM dialogItem;
                HWND groupsLv;
                HWND privilegesLv;

                // This is a big violation of abstraction...

                dialogItem = PhAddPropPageLayoutItem(hwndDlg, hwndDlg,
                    PH_PROP_PAGE_TAB_CONTROL_PARENT, PH_ANCHOR_ALL);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_USER),
                    dialogItem, PH_ANCHOR_LEFT | PH_ANCHOR_TOP | PH_ANCHOR_RIGHT);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_USERSID),
                    dialogItem, PH_ANCHOR_LEFT | PH_ANCHOR_TOP | PH_ANCHOR_RIGHT);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_VIRTUALIZED),
                    dialogItem, PH_ANCHOR_LEFT | PH_ANCHOR_TOP | PH_ANCHOR_RIGHT);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_APPCONTAINERSID),
                    dialogItem, PH_ANCHOR_LEFT | PH_ANCHOR_TOP | PH_ANCHOR_RIGHT);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_GROUPS),
                    dialogItem, PH_ANCHOR_LEFT | PH_ANCHOR_TOP | PH_ANCHOR_RIGHT);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_PRIVILEGES),
                    dialogItem, PH_ANCHOR_ALL);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_INSTRUCTION),
                    dialogItem, PH_ANCHOR_LEFT | PH_ANCHOR_BOTTOM);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_INTEGRITY),
                    dialogItem, PH_ANCHOR_RIGHT | PH_ANCHOR_BOTTOM);
                PhAddPropPageLayoutItem(hwndDlg, GetDlgItem(hwndDlg, IDC_ADVANCED),
                    dialogItem, PH_ANCHOR_RIGHT | PH_ANCHOR_BOTTOM);

                PhDoPropPageLayout(hwndDlg);

                groupsLv = GetDlgItem(hwndDlg, IDC_GROUPS);
                privilegesLv = GetDlgItem(hwndDlg, IDC_PRIVILEGES);

                if (ListView_GetItemCount(groupsLv) != 0)
                {
                    ListView_SetColumnWidth(groupsLv, 0, LVSCW_AUTOSIZE);
                    ExtendedListView_SetColumnWidth(groupsLv, 1, ELVSCW_AUTOSIZE_REMAININGSPACE);
                }

                if (ListView_GetItemCount(privilegesLv) != 0)
                {
                    ListView_SetColumnWidth(privilegesLv, 0, LVSCW_AUTOSIZE);
                    ListView_SetColumnWidth(privilegesLv, 1, LVSCW_AUTOSIZE);
                    ExtendedListView_SetColumnWidth(privilegesLv, 2, ELVSCW_AUTOSIZE_REMAININGSPACE);
                }

                SetProp(hwndDlg, PhMakeContextAtom(), (HANDLE)TRUE);
            }
        }
        break;
    }

    return FALSE;
}
