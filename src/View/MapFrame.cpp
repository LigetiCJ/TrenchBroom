/*
 Copyright (C) 2010-2013 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MapFrame.h"

#include "TrenchBroomApp.h"
#include "View/Console.h"
#include "View/DocumentManager.h"
#include "View/MapView.h"
#include "View/NavBar.h"

#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>

namespace TrenchBroom {
    namespace View {
        IMPLEMENT_DYNAMIC_CLASS(MapFrame, wxFrame)

        MapFrame::MapFrame() :
        wxFrame(NULL, wxID_ANY, wxT("unnamed.map")),
        m_console(NULL),
        m_navBar(NULL),
        m_mapView(NULL) {}

        MapFrame::MapFrame(MapDocument::Ptr document) :
        wxFrame(NULL, wxID_ANY, wxT("unnamed.map")),
        m_console(NULL),
        m_navBar(NULL),
        m_mapView(NULL) {
            Create(document);
            createGui();

            SetSize(1024, 768);
            CenterOnScreen();
        }

        void MapFrame::Create(MapDocument::Ptr document) {
            m_document = document;
            Bind(wxEVT_CLOSE_WINDOW, &MapFrame::OnClose, this);
        }

        MapFrame::~MapFrame() {}

        void MapFrame::OnClose(wxCloseEvent& event) {
            DocumentManager& documentManager = ::documentManager();
            if (!documentManager.closeDocument(m_document))
                event.Veto();
        }

        void MapFrame::createGui() {
            wxSplitterWindow* inspectorSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
            inspectorSplitter->SetSashGravity(1.0f);
            inspectorSplitter->SetMinimumPaneSize(350);
            
            wxSplitterWindow* consoleSplitter = new wxSplitterWindow(inspectorSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
            consoleSplitter->SetSashGravity(1.0f);
            consoleSplitter->SetMinimumPaneSize(0);
            
            m_console = new Console(consoleSplitter);

            wxPanel* container = new wxPanel(consoleSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
#ifdef _WIN32
                                             wxBORDER_SUNKEN
#else
                                             wxBORDER_NONE
#endif
                                             );
            
            m_navBar = new NavBar(container);
            m_mapView = new MapView(container, *m_console);

            wxSizer* containerSizer = new wxBoxSizer(wxVERTICAL);
            containerSizer->Add(m_navBar, 0, wxEXPAND);
            containerSizer->Add(m_mapView, 1, wxEXPAND);
            container->SetSizer(containerSizer);
            
            consoleSplitter->SplitHorizontally(container, m_console, -100);
            wxPanel* inspector = new wxPanel(inspectorSplitter);
            // m_inspector = new Inspector(inspectorSplitter, m_documentViewHolder);
            inspectorSplitter->SplitVertically(consoleSplitter, inspector, -350);
            
            wxSizer* outerSizer = new wxBoxSizer(wxVERTICAL);
            outerSizer->Add(inspectorSplitter, 1, wxEXPAND);
            SetSizer(outerSizer);
        }
    }
}
