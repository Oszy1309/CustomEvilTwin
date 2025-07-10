#include <furi.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>
#include <furi_hal_uart.h>

#define TAG "EvilTwin"

typedef enum {
    EvilTwinViewSubmenu,
    EvilTwinViewTextInput,
    EvilTwinViewPopup,
} EvilTwinView;

typedef struct {
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    TextInput* text_input;
    Popup* popup;
    NotificationApp* notifications;
    char text_buffer[64];
} EvilTwinApp;

// UART zu ESP32 senden
void evil_twin_send_uart(const char* command) {
    FURI_LOG_I(TAG, "Sending: %s", command);
    furi_hal_uart_tx(FuriHalUartIdUSART1, (uint8_t*)command, strlen(command));
    furi_hal_uart_tx(FuriHalUartIdUSART1, (uint8_t*)"\n", 1);
}

// Notification senden
void evil_twin_notify(EvilTwinApp* app, const char* message) {
    popup_set_header(app->popup, "Evil Twin", 64, 16, AlignCenter, AlignBottom);
    popup_set_text(app->popup, message, 64, 32, AlignCenter, AlignTop);
    view_dispatcher_switch_to_view(app->view_dispatcher, EvilTwinViewPopup);
    notification_message(app->notifications, &sequence_success);
}

// Menu Callbacks
void evil_twin_submenu_callback(void* context, uint32_t index) {
    EvilTwinApp* app = context;
    
    switch(index) {
        case 0: // WiFi Scan
            evil_twin_send_uart("START_SCAN");
            evil_twin_notify(app, "WiFi Scan started");
            break;
        case 1: // Set Target
            view_dispatcher_switch_to_view(app->view_dispatcher, EvilTwinViewTextInput);
            break;
        case 2: // Start Deauth
            evil_twin_send_uart("DEAUTH_START");
            evil_twin_notify(app, "Deauth Attack started");
            break;
        case 3: // Stop Deauth
            evil_twin_send_uart("DEAUTH_STOP");
            evil_twin_notify(app, "Deauth Attack stopped");
            break;
        case 4: // Channel Hop
            evil_twin_send_uart("CHANNEL_HOP");
            evil_twin_notify(app, "Channel hopping enabled");
            break;
    }
}

// Text Input Callback (für Target SSID)
void evil_twin_text_input_callback(void* context) {
    EvilTwinApp* app = context;
    
    char command[80];
    snprintf(command, sizeof(command), "SET_TARGET %s", app->text_buffer);
    evil_twin_send_uart(command);
    
    char notify_text[80];
    snprintf(notify_text, sizeof(notify_text), "Target set: %s", app->text_buffer);
    evil_twin_notify(app, notify_text);
}

// Back Callback
uint32_t evil_twin_navigation_exit_callback(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

uint32_t evil_twin_navigation_submenu_callback(void* context) {
    UNUSED(context);
    return EvilTwinViewSubmenu;
}

// App Initialisierung
EvilTwinApp* evil_twin_app_alloc() {
    EvilTwinApp* app = malloc(sizeof(EvilTwinApp));

    // View Dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);

    // Submenu
    app->submenu = submenu_alloc();
    submenu_add_item(app->submenu, "WiFi Scan", 0, evil_twin_submenu_callback, app);
    submenu_add_item(app->submenu, "Set Target", 1, evil_twin_submenu_callback, app);
    submenu_add_item(app->submenu, "Start Deauth", 2, evil_twin_submenu_callback, app);
    submenu_add_item(app->submenu, "Stop Deauth", 3, evil_twin_submenu_callback, app);
    submenu_add_item(app->submenu, "Channel Hop", 4, evil_twin_submenu_callback, app);
    
    view_set_previous_callback(submenu_get_view(app->submenu), evil_twin_navigation_exit_callback);
    view_dispatcher_add_view(app->view_dispatcher, EvilTwinViewSubmenu, submenu_get_view(app->submenu));

    // Text Input
    app->text_input = text_input_alloc();
    text_input_set_header_text(app->text_input, "Enter Target SSID:");
    text_input_set_result_callback(
        app->text_input,
        evil_twin_text_input_callback,
        app,
        app->text_buffer,
        sizeof(app->text_buffer),
        false);
    
    view_set_previous_callback(text_input_get_view(app->text_input), evil_twin_navigation_submenu_callback);
    view_dispatcher_add_view(app->view_dispatcher, EvilTwinViewTextInput, text_input_get_view(app->text_input));

    // Popup
    app->popup = popup_alloc();
    popup_set_timeout(app->popup, 2000);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, NULL);
    
    view_set_previous_callback(popup_get_view(app->popup), evil_twin_navigation_submenu_callback);
    view_dispatcher_add_view(app->view_dispatcher, EvilTwinViewPopup, popup_get_view(app->popup));

    // Notifications
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // UART initialisieren
    furi_hal_uart_set_br(FuriHalUartIdUSART1, 115200);

    return app;
}

void evil_twin_app_free(EvilTwinApp* app) {
    view_dispatcher_remove_view(app->view_dispatcher, EvilTwinViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, EvilTwinViewTextInput);
    view_dispatcher_remove_view(app->view_dispatcher, EvilTwinViewPopup);
    
    submenu_free(app->submenu);
    text_input_free(app->text_input);
    popup_free(app->popup);
    view_dispatcher_free(app->view_dispatcher);
    
    furi_record_close(RECORD_NOTIFICATION);
    
    free(app);
}

// Main Entry Point
int32_t evil_twin_app(void* p) {
    UNUSED(p);
    
    EvilTwinApp* app = evil_twin_app_alloc();
    
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_switch_to_view(app->view_dispatcher, EvilTwinViewSubmenu);
    
    FURI_LOG_I(TAG, "Evil Twin App started");
    evil_twin_send_uart("START_SCAN"); // Auto-scan beim Start
    
    view_dispatcher_run(app->view_dispatcher);
    
    furi_record_close(RECORD_GUI);
    evil_twin_app_free(app);
    
    return 0;
}