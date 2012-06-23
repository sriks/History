import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1
import "HistoryConstants.js" as HistoryConstants;


PageStackWindow {
    id: root
    property QtObject skin;
    property Menu menu;
    property int currentFavIndex: -1;
    property Component toolButtonComponent;
    property Item banner;
    property bool canCreateToolButton;
    property bool dataLoadingError: false;
    signal error(string errorString)

    Component.onCompleted: {
        createBanner();
        var skinComponent = Qt.createComponent("Skin.qml");
        if(Component.Ready  === skinComponent.status)
            skin = skinComponent.createObject(root); // if not created with parent, skin is not getting recognised in device.

        toolButtonComponent = Qt.createComponent("DefaultToolButton.qml");
        if(Component.Ready === toolButtonComponent.status)
            canCreateToolButton = true;
        pageStack.push(Qt.resolvedUrl("LoadingScreen.qml"));
        engine.start();
    }

        QueryDialog {
            id: dialog;
            property bool closeOnAccept;
            titleText: "History";
            onAccepted: {
                if(dialog.closeOnAccept)
                    Qt.quit();
                else if(HistoryConstants.executeOnAcception)
                    HistoryConstants.executeOnAcception();
            }
        }

       Component {
            id: menuComponent
            Menu {
                visualParent: pageStack;
                content: MenuLayout {
                        id: menuLayout;
                            MenuItem { id: showFavList; text: HistoryConstants.menuItemFavoritesText;
                                       onClicked: loadFavList() }
                            MenuItem { text: HistoryConstants.menuItemAboutText; onClicked: showAboutDialog(); }

                            // Makes sense only for symbian
                            //MenuItem { text: HistoryConstants.menuItemExitText; onClicked: Qt.quit() }
                        }

                onStatusChanged: {
                    if(DialogStatus.Opening == status) {
                        if(HistoryConstants.todayPageId == pageStack.currentPage.pageId)
                            showFavList.visible = true;
                        else
                            showFavList.visible = false;
                    }
                }
            }
        }


        onError: {
            dataLoadingError = true;
            loadToday();
        }

        function showAboutDialog() {
            dialog.message = HistoryConstants.aboutText;
            HistoryConstants.executeOnAcception = 0;
            dialog.acceptButtonText = "OK";
            dialog.rejectButtonText = "";
            dialog.open();
        }

        function createBanner() {
            // TODO: get the url in a cross platform manner
            var bannerurl = Qt.resolvedUrl("harmattan/Banner.qml");
            var bannerComponent = Qt.createComponent(bannerurl);
            if(bannerComponent.status === Component.Ready) {
                banner = bannerComponent.createObject(pageStack);
                if(banner)
                    banner.z = pageStack.z + 1;
            }
            else
                console.debug("Banner not created "+bannerComponent.errorString());
        }

        function showBanner(text) {
            if(!banner)
                createBanner();
            banner.text = text;
            banner.show();
        }

        function onUpdateAvailable() {
            dataLoadingError = false;
            loadToday();
        }

        function loadToday() {
            pageStack.push(Qt.resolvedUrl("Today.qml"),{"errorInFetchingData":dataLoadingError});
            currentFavIndex = -1;
        }

        function loadFavList() {
            pageStack.push(Qt.resolvedUrl("FavoritesList.qml"));
            currentFavIndex = -1;
        }

        function loadFavorite(favIndex) {
            // Fix for bug - when user double clicks on a favorite and it loads twice, once without menubar.
            if(pageStack.currentPage.pageId == HistoryConstants.favPageId)
                return;
            currentFavIndex = favIndex;
            var fav = engine.favorite(favIndex);
            pageStack.push(Qt.resolvedUrl("Favorite.qml"));
        }

        function goBack() {
            if( (HistoryConstants.todayPageId == pageStack.currentPage.pageId) ||
                (HistoryConstants.loadingScreenPageId == pageStack.currentPage.pageId) )
                Qt.quit();
            else
                pageStack.pop();
        }

        function showMenuItems() {
            if (!menu)
                menu = menuComponent.createObject(root)
            menu.open();
        }

        function deleteAllFavs() {
            if(!engine.deleteAllFavorites()) {
                banner.text = HistoryConstants.deleteFailedText;
                banner.show();
            }
            else {
                pageStack.currentPage.updatePage();
                banner.text = HistoryConstants.allFavsDeletedText;
                banner.show();
            }
        }

        function deleteFav() {
            if(!engine.deleteFavorite(currentFavIndex)) {
                banner.iconSource = skin.deleteIcon;
                banner.text = HistoryConstants.deleteFailedText;
                banner.show();
            }
            else {
                currentFavIndex = -1;
                goBack(); // goto today view
            }
        }

        function share() {
            if(!engine.share(currentFavIndex)) {
                banner.text = HistoryConstants.unableToShareText;
                banner.iconSource = skin.shareIcon;
                banner.show();
            }
        }

        function handleAction(toolId) {
            if(toolId == HistoryConstants.backId)
                goBack();
            else if(toolId == HistoryConstants.closeId)
                Qt.quit();
            else if(toolId == HistoryConstants.menuId)
                showMenuItems();
            else if(toolId == HistoryConstants.showFavListId)
                loadFavList();
            else if(toolId == HistoryConstants.shareId)
                share();
            else if(toolId == HistoryConstants.saveAsFavId) {
                 banner.iconSource = skin.favIcon;
                 if(!engine.saveAsFavorite())
                     banner.text = HistoryConstants.unableToSaveFavText;
                 else
                     banner.text = HistoryConstants.savedAsFavText;
                 banner.show();
            }
            else if(toolId == HistoryConstants.deleteAllFavsId) {
                dialog.message = HistoryConstants.deleteAllFavsConfirmationText;
                dialog.acceptButtonText = "Delete All";
                HistoryConstants.executeOnAcception = deleteAllFavs;
                dialog.rejectButtonText = "Cancel"
                dialog.open();
            }
            else if(toolId == HistoryConstants.deleteFavId) {
                dialog.message = HistoryConstants.deleteFavConfirmationText;
                dialog.acceptButtonText = "Delete";
                HistoryConstants.executeOnAcception = deleteFav;
                dialog.rejectButtonText = "Cancel"
                dialog.open();
            }
        }

        function createBackToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                tb.iconSource = skin.backIcon;
                tb.toolId = HistoryConstants.backId;
                tb.actionHandler = root;
            }
        }

        function createMenuToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                tb.iconSource = skin.menuIcon;
                tb.toolId = HistoryConstants.menuId
                tb.actionHandler = root;
            }
        }

        function createSaveAsFavToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                tb.iconSource = skin.favIcon;
                tb.toolId = HistoryConstants.saveAsFavId
                tb.actionHandler = root;
            }
        }

        function createDeleteAllFavsToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                //tb.iconSource = skin.deleteIcon;
                tb.platformIconId = "toolbar-delete";
                tb.toolId = HistoryConstants.deleteAllFavsId;
                tb.actionHandler = root;
                return tb;
            }
        }

        function createDeleteFavToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                //tb.iconSource = skin.deleteIcon;
                tb.platformIconId = "toolbar-delete";
                tb.toolId = HistoryConstants.deleteFavId;
                tb.actionHandler = root;
            }
        }

        function createShareToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                //tb.iconSource = skin.shareIcon;
                tb.platformIconId = "toolbar-share";
                tb.toolId = HistoryConstants.shareId;
                tb.actionHandler = root;
            }
        }

        function createCloseToolButton(parent) {
            if(canCreateToolButton) {
                var tb = toolButtonComponent.createObject(parent);
                tb.iconSource = skin.closeIcon;
                tb.toolId = HistoryConstants.closeId;
                tb.actionHandler = root;
            }
        }
}

// eof
