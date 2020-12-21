#ifndef __ODIN_INPUT_H__
#define __ODIN_INPUT_H__


#include <stdbool.h>

/* Forward Dec */
typedef struct odin_window *odin_window;

/** Information about input and devices, like a render device. */
typedef struct odin_input_device *odin_input_device;


/** \brief odin_events are categorized by type to allow for more information to be passed. */
typedef enum odin_event_type
{

    odin_event_type_window = 0x000,
    odin_event_type_keyboard = 0x001,
    odin_event_type_mouse = 0x002,
    odin_event_type_gamepad = 0x003, /* TODO */
    odin_event_type_vr = 0x004  /* TODO */

} odin_event_type;


typedef enum odin_event
{

    odin_event_window_open = 0x0000,
    odin_event_window_close = 0x0001,
    odin_event_window_resize = 0x0002,
    odin_event_window_focus = 0x0003,

    odin_event_keyboard_back_space = 0x1000,
    odin_event_keyboard_tab = 0x1001,
    odin_event_keyboard_return = 0x1001,
    odin_event_keyboard_left_shift = 0x1002,
    odin_event_keyboard_right_shift = 0x1003,
    odin_event_keyboard_left_control = 0x1004,
    odin_event_keyboard_escape = 0x1005,
    odin_event_keyboard_delete = 0x1006,
    odin_event_keyboard_space = 0x1007,
    odin_event_keyboard_exclamation = 0x1008,
    odin_event_keyboard_double_quotes = 0x1009,
    odin_event_keyboard_pound = 0x10A,
    odin_event_keyboard_dollar = 0x101B,
    odin_event_keyboard_percent = 0x101C, 
    odin_event_keyboard_ampersand = 0x101D,
    odin_event_keyboard_quote = 0x101E,
    odin_event_keyboard_open_parenthesis = 0x101F,
    odin_event_keyboard_close_parenthesis = 0x1020,
    odin_event_keyboard_astrick = 0x1021,
    odin_event_keyboard_plus = 0x1022,
    odin_event_keyboard_comma = 0x1023,
    odin_event_keyboard_hyphen = 0x1024,
    odin_event_keyboard_dot = 0x1025,
    odin_event_keyboard_forward_slash = 0x1026,
    odin_event_keyboard_0 = 0x1027,
    odin_event_keyboard_1 = 0x1028,
    odin_event_keyboard_2 = 0x1029,
    odin_event_keyboard_3 = 0x102A,
    odin_event_keyboard_4 = 0x102B,
    odin_event_keyboard_5 = 0x102C,
    odin_event_keyboard_6 = 0x102D,
    odin_event_keyboard_7 = 0x102E,
    odin_event_keyboard_8 = 0x102F,
    odin_event_keyboard_9 = 0x1030,
    odin_event_keyboard_colon = 0x1031,
    odin_event_keyboard_semicolon = 0x1032,
    odin_event_keyboard_less_than = 0x1033,
    odin_event_keyboard_equals = 0x1034,
    odin_event_keyboard_greater_than = 0x1035,
    odin_event_keyboard_quesion_mark = 0x1036,
    odin_event_keyboard_at = 0x1037,
    odin_event_keyboard_A = 0x1038,
    odin_event_keyboard_B = 0x1039,
    odin_event_keyboard_C = 0x103A,
    odin_event_keyboard_D = 0x103B,
    odin_event_keyboard_E = 0x103C,
    odin_event_keyboard_F = 0x103D,
    odin_event_keyboard_G = 0x103E,
    odin_event_keyboard_H = 0x103F,
    odin_event_keyboard_I = 0x1041,
    odin_event_keyboard_J = 0x1042,
    odin_event_keyboard_K = 0x1043,
    odin_event_keyboard_L = 0x1044,
    odin_event_keyboard_M = 0x1045,
    odin_event_keyboard_N = 0x1046,
    odin_event_keyboard_O = 0x1047,
    odin_event_keyboard_P = 0x1048,
    odin_event_keyboard_Q = 0x1049,
    odin_event_keyboard_R = 0x104A,
    odin_event_keyboard_S = 0x104B,
    odin_event_keyboard_T = 0x104C,
    odin_event_keyboard_U = 0x104D,
    odin_event_keyboard_V = 0x104E,
    odin_event_keyboard_W = 0x104F,
    odin_event_keyboard_X = 0x1050,
    odin_event_keyboard_Y = 0x1051,
    odin_event_keyboard_Z = 0x1052,
    odin_event_keyboard_open_bracket = 0x1053,
    odin_event_keyboard_back_slash = 0x1054,
    odin_event_keyboard_close_bracket = 0x1055,
    odin_event_keyboard_caret = 0x1056,
    odin_event_keyboard_underscore = 0x1057,
    odin_event_keyboard_grave_accent = 0x1058,
    odin_event_keyboard_a = 0x1059,
    odin_event_keyboard_b = 0x105A,
    odin_event_keyboard_c = 0x105B,
    odin_event_keyboard_d = 0x105C,
    odin_event_keyboard_e = 0x105D,
    odin_event_keyboard_f = 0x105E,
    odin_event_keyboard_g = 0x105F,
    odin_event_keyboard_h = 0x1060,
    odin_event_keyboard_i = 0x1061,
    odin_event_keyboard_j = 0x1062,
    odin_event_keyboard_k = 0x1063,
    odin_event_keyboard_l = 0x1064,
    odin_event_keyboard_m = 0x1065,
    odin_event_keyboard_n = 0x1066,
    odin_event_keyboard_o = 0x1067,
    odin_event_keyboard_p = 0x1068,
    odin_event_keyboard_q = 0x1069,
    odin_event_keyboard_r = 0x106A,
    odin_event_keyboard_s = 0x106B,
    odin_event_keyboard_t = 0x106C,
    odin_event_keyboard_u = 0x106D,
    odin_event_keyboard_v = 0x106E,
    odin_event_keyboard_w = 0x106F,
    odin_event_keyboard_x = 0x1070,
    odin_event_keyboard_y = 0x1071,
    odin_event_keyboard_z = 0x1072,
    odin_event_keyboard_open_braces = 0x1073,
    odin_event_keyboard_vertical_slash = 0x1074,
    odin_event_keyboard_close_braces = 0x1075,
    odin_event_keyboard_tilde = 0x1076,
    odin_event_keyboard_insert = 0x1078,
    odin_event_keyboard_home = 0x1079,
    odin_event_keyboard_page_up = 0x107A,
    odin_event_keyboard_end = 0x107B,
    odin_event_keyboard_page_down = 0x107C,
    odin_event_keyboard_F1 = 0x107D,
    odin_event_keyboard_F2 = 0x107E,
    odin_event_keyboard_F3 = 0x107F,
    odin_event_keyboard_F4 = 0x1080,
    odin_event_keyboard_F5 = 0x1081,
    odin_event_keyboard_F6 = 0x1082,
    odin_event_keyboard_F7 = 0x1083,
    odin_event_keyboard_F8 = 0x1084,
    odin_event_keyboard_F9 = 0x1085,
    odin_event_keyboard_F10 = 0x1086,
    odin_event_keyboard_F11 = 0x1087,
    odin_event_keyboard_F12 = 0x1088,
    odin_event_keyboard_F13 = 0x1089,
    odin_event_keyboard_F14 = 0x108A,
    odin_event_keyboard_F15 = 0x108B,
    odin_event_keyboard_F16 = 0x108C,
    odin_event_keyboard_F17 = 0x108D,
    odin_event_keyboard_F18 = 0x108E,
    odin_event_keyboard_F19 = 0x108F,
    odin_event_keyboard_F20 = 0x1090,
    odin_event_keyboard_F21 = 0x1091,
    odin_event_keyboard_F22 = 0x1092,
    odin_event_keyboard_F23 = 0x1093,
    odin_event_keyboard_F24 = 0x1094,
    odin_event_keyboard_F25 = 0x1095,


    odin_event_mouse_left_button = 0x2000,
    odin_event_mouse_middle_button = 0x2001,
    odin_event_mouse_right_button = 0x2002,
    odin_event_mouse_scroll = 0x2003,
    odin_event_mouse_extra_button_1 = 0x2004,
    odin_event_mouse_extra_button_2 = 0x2005

    /* TODO: Add gamepad events. */





} odin_event;

typedef enum odin_button_state
{

    odin_button_state_down = 0x0001,
    odin_button_state_up = 0x0002

} odin_button_state;

typedef enum odin_keyboard_modifier
{

    odin_keyboard_modifier_shift = 0x0001,
    odin_keyboard_modifier_control = 0x0002,
    odin_keyboard_modifier_alt = 0x0003,
    odin_keyboard_modifier_super = 0x0004

} odin_keyboard_modifier;


typedef struct odin_event_data *odin_event_data;

typedef struct odin_event_window_data
{

    int width;
    int height;
    bool focused;

} odin_event_window_data_t, *odin_event_window_data;

typedef struct odin_event_keyboard_data
{

    odin_button_state key_state;
    odin_keyboard_modifier modifier_flags;

} odin_event_keyboard_data_t,  *odin_event_keyboard_data;

typedef struct odin_event_mouse_data
{

    int delta_x;
    int delta_y;

} odin_event_mouse_data_t,  *odin_event_mouse_data;

typedef struct odin_event_gamepad_data
{
    
    int TODO;

} *odin_event_gamepad_data;


/* This should only be used internally. */
typedef void ( *odin_input_window_events_processor)(odin_window window, odin_event event, odin_event_window_data_t window_data);
typedef void ( *odin_input_keyboard_events_processor)(odin_window window, odin_event event, odin_event_keyboard_data_t keyboard_data);
typedef void ( *odin_input_mouse_events_processor)(odin_window window, odin_event event, odin_event_mouse_data_t mouse_data);



/** \brief Creates an input device. */
void odin_input_device_create(odin_input_device* input_device);

/** \brief Destroys an input device. */
void odin_input_device_destroy(odin_input_device input_device);



void odin_input_device_set_window_processor (odin_input_device input_device, odin_input_window_events_processor window_processor);
void odin_input_device_set_keyboard_processor(odin_input_device input_device, odin_input_keyboard_events_processor keyboard_processor);
void odin_input_device_set_mouse_processor(odin_input_device input_device, odin_input_mouse_events_processor mouse_processor);


/** \brief Polls the information from the windows. */
void odin_input_poll();



#endif /* __ODIN_INPUT_H__ */