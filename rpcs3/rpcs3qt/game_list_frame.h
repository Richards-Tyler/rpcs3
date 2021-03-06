#pragma once

#include "stdafx.h"
#include "Emu/GameInfo.h"

#include "game_list_grid.h"
#include "gui_settings.h"
#include "emu_settings.h"

#include <QDockWidget>
#include <QList>
#include <QTableWidget>
#include <QMainWindow>
#include <QToolBar>
#include <QLineEdit>
#include <QStackedWidget>

#include <memory>

enum Category
{
	Disc_Game,
	Non_Disc_Game,
	Home,
	Media,
	Data,
	Unknown_Cat,
	Others,
};

namespace category // (see PARAM.SFO in psdevwiki.com) TODO: Disc Categories 
{
	// PS3 bootable
	const QString app_Music = QObject::tr("App Music");
	const QString app_Photo = QObject::tr("App Photo");
	const QString app_TV    = QObject::tr("App TV");
	const QString app_Video = QObject::tr("App Video");
	const QString bc_Video  = QObject::tr("Broadcast Video");
	const QString disc_Game = QObject::tr("Disc Game");
	const QString hdd_Game  = QObject::tr("HDD Game");
	const QString home      = QObject::tr("Home");
	const QString network   = QObject::tr("Network");
	const QString store_FE  = QObject::tr("Store");
	const QString web_TV    = QObject::tr("Web TV");

	// PS2 bootable
	const QString ps2_game = QObject::tr("PS2 Classics");
	const QString ps2_inst = QObject::tr("PS2 Game");

	// PS1 bootable
	const QString ps1_game = QObject::tr("PS1 Classics");

	// PSP bootable
	const QString psp_game = QObject::tr("PSP Game");
	const QString psp_mini = QObject::tr("PSP Minis");
	const QString psp_rema = QObject::tr("PSP Remasters");

	// Data
	const QString ps3_Data = QObject::tr("PS3 Game Data");
	const QString ps2_Data = QObject::tr("PS2 Emulator Data");

	// Save
	const QString ps3_Save = QObject::tr("PS3 Save Data");
	const QString psp_Save = QObject::tr("PSP Minis Save Data");

	// others
	const QString trophy  = QObject::tr("Trophy");
	const QString unknown = QObject::tr("Unknown");
	const QString other   = QObject::tr("Other");

	const q_from_char cat_boot =
	{
		{ "AM",app_Music }, // media
		{ "AP",app_Photo }, // media
		{ "AT",app_TV },    // media
		{ "AV",app_Video }, // media
		{ "BV",bc_Video },  // media
		{ "DG",disc_Game }, // disc_Game
		{ "HG",hdd_Game },  // non_disc_games
		{ "HM",home },      // home
		{ "CB",network },   // other
		{ "SF",store_FE },  // other
		{ "WT",web_TV },    // media
		{ "2P",ps2_game },  // non_disc_games
		{ "2G",ps2_inst },  // non_disc_games
		{ "1P",ps1_game },  // non_disc_games
		{ "PP",psp_game },  // non_disc_games
		{ "MN",psp_mini },  // non_disc_games
		{ "PE",psp_rema }   // non_disc_games
	};
	const q_from_char cat_data =
	{
		{ "GD",ps3_Data }, // data
		{ "2D",ps2_Data }, // data
		{ "SD",ps3_Save }, // data
		{ "MS",psp_Save }  // data
	};

	const QStringList non_disc_games = { hdd_Game, ps2_game, ps2_inst, ps1_game, psp_game, psp_mini, psp_rema };
	const QStringList media = { app_Photo, app_Video, bc_Video, app_Music, app_TV, web_TV };
	const QStringList data = { ps3_Data, ps2_Data, ps3_Save, psp_Save };
	const QStringList others = { network, store_FE, trophy, other };
}

/* Having the icons associated with the game info simplifies logic internally */
typedef struct GUI_GameInfo
{
	GameInfo info;
	QImage icon;
	QPixmap pxmap;
	bool bootable;
};

typedef struct Tool_Bar_Button
{
	QAction* action;
	QIcon colored;
	QIcon gray;
};

class game_list_frame : public QDockWidget {
	Q_OBJECT

public:
	explicit game_list_frame(std::shared_ptr<gui_settings> settings, Render_Creator r_Creator, QWidget *parent = nullptr);
	~game_list_frame();
	void Refresh(const bool fromDrive = false);
	void ToggleCategoryFilter(const QStringList& categories, bool show);

	/** Loads from settings. Public so that main frame can easily reset these settings if needed. */
	void LoadSettings();

	/** Saves settings. Public so that main frame can save this when a caching of column widths is needed for settings backup */
	void SaveSettings();

public Q_SLOTS:
	/** Resize Gamelist Icons to size */
	void ResizeIcons(const QSize& size, const int& idx);
	void SetListMode(const bool& isList);
	void SetToolBarVisible(const bool& showToolBar);
	void SetCategoryActIcon(const int& id, const bool& active);

private Q_SLOTS:
	void Boot(int row);
	void RemoveCustomConfiguration(int row);
	void OnColClicked(int col);

	void ShowContextMenu(const QPoint &pos);
	void ShowSpecifiedContextMenu(const QPoint &pos, int index); // Different name because the notation for overloaded connects is messy
	void doubleClickedSlot(const QModelIndex& index);
Q_SIGNALS:
	void game_list_frameClosed();
	void RequestIconPathSet(const std::string path);
	void RequestAddRecentGame(const q_string_pair& entry);
	void RequestIconSizeActSet(const int& idx);
	void RequestListModeActSet(const bool& isList);
	void RequestCategoryActSet(const int& id);
protected:
	/** Override inherited method from Qt to allow signalling when close happened.*/
	void closeEvent(QCloseEvent* event);
	void resizeEvent(QResizeEvent *event);
private:
	void PopulateGameGrid(uint maxCols, const QSize& image_size);
	void FilterData();

	int PopulateGameList();
	bool SearchMatchesApp(const std::string& name, const std::string& serial);

	std::string CurrentSelectionIconPath();

	// Which widget we are displaying depends on if we are in grid or list mode.
	QMainWindow* m_Game_Dock;
	QStackedWidget* m_Central_Widget;
	QToolBar* m_Tool_Bar;
	QLineEdit* m_Search_Bar;
	QSlider* m_Slider_Size;
	QTableWidget *gameList;
	game_list_grid* m_xgrid;

	// Actions regarding showing/hiding columns
	QAction* showIconColAct;
	QAction* showNameColAct;
	QAction* showSerialColAct;
	QAction* showFWColAct;
	QAction* showAppVersionColAct;
	QAction* showCategoryColAct;
	QAction* showPathColAct;

	QList<QAction*> columnActs;

	// Actions regarding showing/hiding categories
	Tool_Bar_Button m_catActHDD;
	Tool_Bar_Button m_catActDisc;
	Tool_Bar_Button m_catActHome;
	Tool_Bar_Button m_catActGameData;
	Tool_Bar_Button m_catActAudioVideo;
	Tool_Bar_Button m_catActUnknown;
	Tool_Bar_Button m_catActOther;

	QList<Tool_Bar_Button> m_categoryButtons;

	QActionGroup* m_categoryActs;

	// Actions regarding switching list modes
	Tool_Bar_Button m_modeActList;
	Tool_Bar_Button m_modeActGrid;

	QActionGroup* m_modeActs;

	// TODO: Reorganize this into a sensible order for private variables.
	std::shared_ptr<gui_settings> xgui_settings;

	int m_sortColumn;
	Qt::SortOrder m_colSortOrder;
	bool m_isListLayout = true;
	bool m_oldLayoutIsList = true;
	bool m_showToolBar = true;
	std::vector<GUI_GameInfo> m_game_data;
	QSize m_Icon_Size;
	QString m_Icon_Size_Str;
	qreal m_Margin_Factor;
	qreal m_Text_Factor;
	QStringList m_categoryFilters;
	Render_Creator m_Render_Creator;

	uint m_games_per_row = 0;
};
