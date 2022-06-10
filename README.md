# CS 1.6 External cheat
External cheat for CS 1.6<br>
Tested on:<br>
Exe version 1.1.2.7/Stdio (cstrike)<br>
Exe build: 15:17:55 Jul 24 2019 (8308)<br>
<br>
<b>Features:</b><br>
- Aimbot<br>
- Box esp<br>
- Distance esp<br>
- Name esp<br>
- BunnyHop<br>
- DDRun<br>
- AutoPistol<br>
- FPS Unlock<br>
<br>
<b>Bypass:</b><br>
- Wargods (need to close the cheat before scanning)<br>
- Sxe <br>
- all server anti-cheats<br>
- prob some other trash<br>
<br>
<b>Information about GameGuard:</b><br>
- All WinApi are logged.<br>
- All connections are logged.<br>
- Threads seems detected.<br>
- mouse_event is detected.<br>
- All drivers are logged.<br>
- You will need a kernel driver to read game memory.<br>
- They have full access to your PC<br>
- They are very evil and mean. You will not defeat them!<br>

<h1>Offsets and patterns</h1>
<br><b>Updated offsets and patterns for Exe build: 19:53:27 Aug  3 2020 (8684)</b><br>
	offsets::MoveType = Engine + 0x100A0B8;		//520 On gounrd, 8 in air, 24 in water, 536 in watter onground. 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 08 02 00 00 00 00 00 00<br>
	offsets::OnGround = Engine + 0x122E2D4;		//1 On Ground, 0 in air 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 38 44<br>
	offsets::OnLadder = Client + 0x136D8C;		//1 on ladder, 0 on ground //00 00 00 00 01 00 00 00 00 A0 84 04 BD 02 00 00 00 00 00 00 01 00 00 00 + 0x10<br>
	offsets::IsChatting = Engine + 0x64429C;	//1 when chat is enabled //01 00 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00<br>
	offsets::InMenu = Engine + 0x6C3AB0;		//01 00 00 00 00 00 00 00 00 00 00 00 80 02 00 00 E0 01 00 00 00 00 00 00<br>
	offsets::InCross = Client + 0x125314;		//00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 B0 97 <br>
	offsets::ViewMatrix = Client + 0x12EAF0;	//00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 3F 00 00 80 3F + 0x40<br>
	offsets::Recoil = Engine + 0x122E324;		//00 00 00 00 00 00 00 00 00 00 00 00 20 00 00 00 01 00 00 00 00 00 00 00<br>
	offsets::EntityList = Engine + 0x12043C8;	//22 00 00 00 5C 62 6F 74 74 6F 6D 63 6F 6C 6F 72 5C 36 5C 63 6C 5F 64 6C<br>
<br><b>PlayerInfo</b><br>
struct PlayerInfo<br>
{<br>
	__int32 number; //0x0 0<br>
	char cmd[256]; //0x4 4<br>
	char name[44]; //0x104 260<br>
	char model[80];//0x130 304<br>
	float anim_frame; //0x180 384<br>
	float smth;			//0x184 388<br>
	Vector3 position;	//0x188 392<br>
	char pad_0x0180[188]; <br>
};<br><br><br>
<b>View pattern</b><br>
0 0 0 0 0 0 0 128 0 0 128 63 213 5 143 189 255 95 127 191 0 0 0 128<br>
0 0 0 0 0 0 0 0 0 0 128 63 212 197 49 62 217 28 124 191 0 0 0 128<br>
0 0 0 0 0 0 0 128 0 0 128 63 213 5 143 189 255 95 127 191 0 0 0 128<br>
<br>
client.dll+12D9F0 hw.dll+1230274 hw.dll+EC7AB0 hw.dll+A2E4C0 hw.dll+7C7840<br>
<br>
<b>fovscale 1  (1.000000119)</b><br>
1 0 128 63 0 0 0 0 0 0 0 0 0 0<br>
171 214 47 64 0 0 0 0 0 0 0 0 0 0  //scoped (2.747477293)<br>
 
<b>fovscale 2 (1.333333492)</b><br>
172 170 170 63 0 0 0 0 0 0 <br>
143 115 106 64 0 0 0 0 0 0 //scoped (3.663303137)<br>

<b>recoil</b><br>
0 0 0 0 0 0 0 0 0 0 0 0 32 0 0 0 1 0 0 0 0 0 0 0<br>

<br>
<h1>The story behind</h1><br>
I've coded it a long time ago. First release was for MYAC (old fastcup anticheat). They had a giant hole that allows you to view/edit game memory through steam.
Then they updated the anticheat and I was forced to use a kernel driver. I sold a few copies and then one guy went semi-rage (https://www.youtube.com/watch?v=Ze6tQYku9BU).
You can see that he is using my creepy shaky aimbot (he edited smoothly in a config so he used lower smooth than possible to configure by cheat). After like 35 days, admins viewed his logs and all users including me were banned. 
After that I received some "popularity". Army of scammers used my media and my name to scam people. Turkish scammers treated me to cut off my head. Hundreds of beggars wrote to me to give them a cheat.
After half of the year I had 300+ people in my block list (and people still coming). I tried to delete all information about me and fastcup, but it didn't work, plus scammers started to claim my work. You may call this "success", but I never seen anything like that before. 
Those people almost completely did not understand human speech. They can't follow basic orders. I tried to explain to them that I have no cheats and I am not doing cheats. Later I managed to spoof my system and recode the cheat. But those people...
Swear to god, I never seen anything like that before. 90% of them were just beggars and wanted the cheat for free. Some of them, after we agreed for conditions and price, they transferred 2 times more money to scammers (so they paid 3000RUB to the scammers instead of paying me 1500 even though they knew that I am a coder and owner of the project), and then they came back and argue with me.
Some of them, after talking with me for days, said "Well, I'll take it for 100 rubles. (which evals nothing)" even after we discussed the price 10 times. Others completely did not understand where they were texting and were communicating like cave people, "Fastcup. Cheat. Want".
I thought it was a troll army, but no. They were real people of different ages and hobbies. I realized that software development does not make sense, since the cheat will get leaked or detected very quickly and it is impossible to get any profit from that because anti-cheat developers are much smarter and more professional than me.
But I started research to understand how those people lost their humanity and ability to think/speak/understand information. I discovered that fastcup owner, Max Pain, is a Russian bandit who makes money by cheating children and destroying their future by instilling gambling addiction. He is an owner of EasyDrop (as well as ForceDrop, TopSkin, EasyDota, MyDota, CSCard and CSGONET) - scam case roulette with 0% chanse victory.
I got this information from persons who worked both with Max Pain and SAH4R SHOW (A YouTuber with 3M subscriptions). You can proof it by checking his profiles. He got links to easydrop. You can also check it by checking website information and scripts.
I continued my search and I found out that Fastcup is not a noncommercial playground. <b>FASTCUP IS A SECRET PROJECT FOR BRAINWASHING SPONSORED BY RUSSIAN GOVERNMENT.</b> While you playing games, they send you special codes using noise on special frequency and showing semi-transparent image codes in a corners of a screen where you cant see them, but your brains still reads info from it.
When a writing program is done, it can be activated any moment. When activated, the victim goes into a trance and moves to the designated location where special people meet him.
They move the victim to a special laboratory where non-vital brain lobes are removed. They then envelop the victim with radiation using graphite rods from the Chernobyl Nuclear Power Plant in order to degrade the central nervous system.
Final part - they install a special chip into the victim's brain that will receive and execute special programs when needed.  
Fastcup strips its victim of mind and reason, leaving only an empty, mindless shell. The brainless bodies then turn into zombies, following only the most basic instincts.
This madness should be stopped. Someone needs to destroy this empire of evil.<br>
<h1>История</h1><br>
Я написал его довльно давно. Первый релиз был для MYAC (старый античит fastcup).
У них была гигантская дыра, которая позволяла просматривать / редактировать игровую память через Steam. 
Потом обновили античит, и я был вынужден использовать драйвер. Я продал несколько копий, а потом один парень пошел катать по семи рейджу (https://www.youtube.com/watch?v=Ze6tQYku9BU). 
Вы можете видеть, что он использует мой кривой аимбот (через конфиг он поставил smooth меньше чем можно было читом). Примерно через 35 дней администраторы просмотрели его логи, и все пользователи, включая меня, были забанены.
После этого я получил некоторую «популярность». Армия мошенников использовала мои медии и мое имя, чтобы обманывать людей. 
Турецкие мошенники угрожали мне отрубить голову. Сотни попрошаек писали мне, чтобы я дал им чит. 
Спустя полгода в моем ЧС было более 300 человек (и люди все еще пишут). 
Я попытался удалить всю информацию обо мне и fastcup, но ничего не вышло, они продолжали писать, к тому же мошенники начали присваивать себе мою работу. 
Вы можете назвать это «успехом», но я никогда раньше не видел ничего подобного. Эти люди почти полностью не понимали человеческую речь.
Они не могут выполнять базове указания. Я пытался объяснить им, что у меня нет читов и я не занимаюсь читами. Они не воспринимали это.
Позже мне удалось переписать чит. Но эти люди ... Богом клянусь, я никогда раньше не видел ничего подобного. 
90% из них были просто попрошайками и хотели чит бесплатно. Некоторые из них после того, как мы согласовали условия и цену, перечисляли мошенникам в 2 раза больше денег (таким образом, они платили мошенникам 3000 руб. вместо того, чтобы заплатить мне 1500, хотя они знали, что я кодер и владелец проекта), и потом они возвращались и ругались со мной. 
Некоторые из них, поговорив со мной несколько дней, говорили: «Ну, я возьму за 100 руб.» даже после того, как мы обсуждали цену 10 раз. Остальные совершенно не понимали, куда пишут, и общались, как пещерные люди: «Фасткап. Чит. Хотеть». Я думал, что это армия троллей, но нет. 
Это были настоящие люди разного возраста и увлечений. Я понял, что разработка софта не имеет смысла, так как чит сразу сольют или задетектят, и невозможно получить прибыль от этого, потому что разработчики античитов намного умнее и профессиональнее меня. 
Но я начал исследование, чтобы понять, как эти люди утратили человечность и способность думать / говорить / понимать информацию. Я обнаружил, что владелец fastcup Макс Пейн - русский бандит, который зарабатывает деньги, обманывая детей и разрушая их будущее, прививая зависимость от азартных игр.
Он владелец EasyDrop (а также ForceDrop, TopSkin, EasyDota, MyDota, CSCard и CSGONET) - мошеннической рулетки с нулевым шансом на победу. Я получил эту информацию от человека, который работал как с Max Pain, так и с SAH4R SHOW (ютубер с 3 милионами подпичсиков). 
Вы можете проверить это, чекнув его профили. У него есть ссылки на easydrop. Вы также можете проверить это, проверив информацию на сайте и скрипты. 
Я продолжил поиск и обнаружил, что Fastcup - это не некоммерческая площадка. FASTCUP - СЕКРЕТНЫЙ ПРОЕКТ ДЛЯ ПРОМЫВКИ МОЗГА, СПОНСИРУЕМЫЙ ПРАВИТЕЛЬСТВОМ РОССИИ.
Пока вы играете в игры, они отправляют вам специальные коды, используя шум на определенной частоте и показывая полупрозрачные коды изображений в углах экрана, где вы их не видите,
но ваш мозг все равно считывает информацию с них. Когда написание программы закончено, ее можно активировать в любой момент.
При активации жертва входит в транс и идет в указанное место, где его встречают особые люди. 
Жертву перемещают в специальную лабораторию, где удаляют нежизненоважные доли мозга. Затем они облчают жертву радиацией с помощью графитовых стержней с Чернобыльской АЭС, чтобы разрушить центральную нервную систему. 
Заключительная часть - в мозг жертвы устанавливают специальный чип, который при необходимости принимает и запускает специальные программы. 
Fastcup лишает жертву разума, оставляя только пустую, бессмысленную скорлупу. Затем безмозглые тела превращаются в зомби, следуя только самым основным инстинктам.
Это безумие нужно остановить. Кому-то нужно разрушить эту империю зла.
