//
// Created by faffo on 26/01/16.
//

#include <libmng_types.h>
#include <gtk/gtk.h>

const int LENGTH = 50;
//const int INTMAX = 2100;
/*
enum t_status {
    ongoing = 0,
    concluded
};
 */
/*enum t_genre {
    horror = 0,
    fantasy,
    political,
    scifi,
    sitcom,
    soap,
    thriller
};
*/
enum tm_columns {
    COL_NAME = 0,
    COL_LAST_EP,
    COL_N_EP,
    COL_N_S,
    COL_YEAR,
    COL_GEN,
    COL_STAT,
    COL_WATCHED
};

enum edit_mode {
    DEL_MODE = 0,
    EDIT_MODE
};

struct series {
    char name[LENGTH];
    char last_episode[LENGTH];
    gint n_episodes;
    gint n_seasons;
    gint year;
    char genre[LENGTH];
    int status;
    int watched;
};

struct n_search_gtk{
    char *name;
    int mode;
    series *edited;
};
