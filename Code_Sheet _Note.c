//Sheet note 
//Pokemon victory soundtrack
uint16_t sheetnoteWin[] = {B_4,MUTE,A_4,MUTE,Ab_4,MUTE,A_4,B_4,MUTE,B_4,MUTE,
	                         B_4,B_4,Db_5,Db_5,MUTE,Db_5,Db_5,MUTE,B_4,MUTE,B_4,MUTE,B_4,B_4,
                           Db_5,MUTE,Db_5,MUTE,Db_5,Db_5,B_4,B_4,MUTE};
uint16_t cur_music_win = 0;

//Kirby defeat                    								 
uint16_t sheetnoteLose[] = {C_6,MUTE,B_5,MUTE,B_5,MUTE,A_5,MUTE,A_5,MUTE,E_5,G_5,MUTE,MUTE,G_6,MUTE};
uint16_t cur_music_lose = 0;

//Custom music 
uint16_t sheetnoteDraw[] = {C_5,G_5,C_5,G_5};
uint16_t cur_music_draw = 0;
												
uint16_t sheetnote_count[] = {D_4,D_5,G_5,E_6};
uint16_t cur_count = 0;