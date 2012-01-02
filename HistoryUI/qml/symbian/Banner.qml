import QtQuick 1.1
import com.nokia.extras 1.0

// Symbian specific InfoBanner
InfoBanner {
     id: banner
     iconSource: skin.favIcon;
     timeout: 3000; // same as harmattan

     // Provide show() to align with harmattan InfoBanner api.
     function show() {
         open();
     }
}
