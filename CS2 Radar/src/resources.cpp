#include "gui.h"

void GUI::loadMaps() {
	maps["cs_italy"] = map(-2647.0f, 2592.0f, 4.6f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\cs_italy_radar.png"));
	maps["cs_office"] = map(-1838.0f, 1858.0f, 4.1f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\cs_office_radar.png"));
	maps["de_ancient"] = map(-2953.0f, 2164.0f, 5.0f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_ancient_radar.png"));
	maps["de_anubis"] = map(-2796.0f, 3328.0f, 5.22f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_ancient_radar.png"));
	maps["de_dust2"] = map(-2476.0f, 3239.0f, 4.4f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_dust2_radar.png"));
	maps["de_inferno"] = map(-2087.0f, 3870.0f, 4.9f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_inferno_radar.png"));
	maps["de_mirage"] = map(-3230.0f, 1713.0f, 5.0f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_mirage_radar.png"));
	maps["de_nuke_lower"] = map(-3453.0f, 2887.0f, 7.0f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_nuke_lower_radar.png"));
	maps["de_nuke"] = map(-3453.0f, 2887.0f, 7.0f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_nuke_radar.png"));
	maps["de_overpass"] = map(-4831.0f, 1781.0f, 5.2f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_overpass_radar.png"));
	maps["de_vertigo_lower"] = map(-3168.0f, 1762.0f, 4.0f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_vertigo_lower_radar.png"));
	maps["de_vertigo"] = map(-3168.0f, 1762.0f, 4.0f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_vertigo_radar.png"));
	maps["de_train"] = map(-2308.0f, 2078.0f, 4.082077f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_train_radar.png"));
	maps["de_cache"] = map(-2000.0f, 3250.0f, 5.5f, LoadImageTexture(g_pd3dDevice, L".\\textures\\maps\\de_cache_radar.png"));

}

void GUI::loadIcons() {
	icons[1] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\deagle.png"));
	icons[2] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\elite.png"));
	icons[3] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\fiveseven.png"));
	icons[4] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\glock.png"));
	icons[7] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\ak47.png"));
	icons[8] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\aug.png"));
	icons[9] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\awp.png"));
	icons[10] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\famas.png"));
	icons[11] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\g3sg1.png"));
	icons[13] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\galilar.png"));
	icons[14] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\m249.png"));
	icons[16] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\m4a1.png"));
	icons[17] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\mac10.png"));
	icons[19] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\p90.png"));
	icons[23] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\mp5sd.png"));
	icons[24] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\ump45.png"));
	icons[25] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\xm1014.png"));
	icons[26] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\bizon.png"));
	icons[27] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\mag7.png"));
	icons[28] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\negev.png"));
	icons[29] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\sawedoff.png"));
	icons[30] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\tec9.png"));
	icons[31] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\taser.png"));
	icons[32] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\hkp2000.png"));
	icons[33] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\mp7.png"));
	icons[34] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\mp9.png"));
	icons[35] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\nova.png"));
	icons[36] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\p250.png"));
	icons[38] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\scar20.png"));
	icons[39] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\sg556.png"));
	icons[40] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\ssg08.png"));
	icons[41] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knifegg.png"));
	icons[42] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife.png"));
	icons[43] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\flashbang.png"));
	icons[44] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\hegrenade.png"));
	icons[45] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\smokegrenade.png"));
	icons[46] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\molotov.png"));
	icons[47] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\decoy.png"));
	icons[48] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\incgrenade.png"));
	icons[49] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\c4.png"));
	icons[55] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\defuser.png"));
	icons[59] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_t.png"));
	icons[60] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\m4a1_silencer.png"));
	icons[61] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\usp_silencer.png"));
	icons[63] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\cz75a.png"));
	icons[64] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\revolver.png"));
	icons[500] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\bayonet.png"));
	icons[503] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_css.png"));
	icons[505] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_flip.png"));
	icons[506] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_gut.png"));
	icons[507] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_karambit.png"));
	icons[508] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_m9_bayonet.png"));
	icons[509] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_tactical.png"));
	icons[512] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_falchion.png"));
	icons[514] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_survival_bowie.png"));
	icons[515] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_butterfly.png"));
	icons[516] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_push.png"));
	icons[517] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_cord.png"));
	icons[518] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_canis.png"));
	icons[519] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_ursus.png"));
	icons[520] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_gypsy_jackknife.png"));
	icons[521] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_outdoor.png"));
	icons[522] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_stiletto.png"));
	icons[523] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_widowmaker.png"));
	icons[525] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_skeleton.png"));
	icons[526] = icon(LoadImageTexture(g_pd3dDevice, L".\\textures\\icons\\knife_kukri.png"));
}