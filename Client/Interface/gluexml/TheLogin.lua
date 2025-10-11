
function PageLogin(Objs)
	local ZoneAlert = 0;
	local ZoneLogin = 0;
	local PageAlert = "";
	local nbrObjet = 0;
	
	ZoneAlert = strfind(Objs,'PAGE:');
	ZoneAlert = ZoneAlert or 0;
	
	--pligne = pligne + 1;
	--ligne=ligne .. pligne .. " : Objs "..Objs;
	
	if(ZoneAlert > 0) then
		PageAlert = strsub(Objs,1,ZoneAlert-1);
		ZoneLogin = strfind(Objs,'<html><body>');
		ZoneLogin = ZoneLogin or 0;
		
		if(strsub(PageAlert,1,4) == "VIDE" or ZoneLogin == 0) then
			PageAlert = "";
		else
			PageAlert = strsub(Objs,ZoneLogin);
		end
		Objs = strsub(Objs,ZoneAlert,ZoneLogin-1);

		_,_, MainMenu = strfind(Objs, "|HMain%[(.-)%]");
		
		MainMenu = MainMenu or "Vide";

		if (MainMenu == "Vide")then

			local Bcl = 1;
			local OldTest = "";
			local is = "";
			
			
			while (Bcl > 0) do
				local S = "|H"..Bcl.."%[(.-)%]";

				local Test = {};
				local _,_, ObjTest = strfind(Objs, S);
				
				ObjTest = ObjTest or "Vide";
				if(ObjTest ~= "Vide") then
					
					Test[1], Test[2], Test[3], Test[4], Test[5], Test[6], Test[7], Test[8], Test[9], Test[10], Test[11], Test[12],Test[13], Test[14], Test[15]  = strsplit(",", ObjTest);
					Test[1]=tonumber(Test[1]) or 0;
					Test[2]=tonumber(Test[2]) or 0;
					Test[3]=tonumber(Test[3]) or 0;
					Test[4]=tonumber(Test[4]) or 0;
					Test[5]=tonumber(Test[5]) or 0;
					Test[6]=tonumber(Test[6]) or 0;
					Test[7]=tonumber(Test[7]) or 0;
					Test[8]=Test[8] or "";
					Test[9]=tonumber(Test[9]) or 0;
					Test[10]=tonumber(Test[10]) or 0;
					Test[11]=tonumber(Test[11]) or 0;
					Test[12]=Test[12] or "";
					Test[13]=Test[13] or "";
					Test[14]=Test[14] or "";
					Test[15]=Test[15] or "";

					if (Test[12] == "m") then
						Test[12] = OldTest;
					elseif (Test[12] > "") then
						OldTest = Test[12];
					end

					_,is = strsplit(".", Test[12]);
					if (not is) then
						Test[12] = Test[12]..".m2";
					end					

					if (Test[14] == "m") then
						Test[14] = Test[12];
					end
					
					--local lig = Test[14] or "vide";
							--ligne = ligne .. " Test[14] = "..lig.."\n";
							
					local ii = 0;
					for ii=8,15,1 do
						if (Test[ii] == "") then
							Test[ii]=nil
						end
					end
					
					table.insert(ModelList, Test);
					
					nbrObjet = nbrObjet + 1;
					Bcl = Bcl + 1;
				else
					Bcl = 0;
				end
			end
			
			
			
			if(nbrObjet > 0) then
				local _,_, ObjTest = strfind(Objs, "|G%[(.-)%]");
				ObjTest = ObjTest or "Vide";

				if(ObjTest ~= "Vide") then
					local BGround = {};
					local Position = {};
					local Cam;
					
					Cam, Position[1], Position[2], Position[3], Position[4], Position[5], Position[6]  = strsplit(",", ObjTest);

					LoginCam = tonumber(Cam) or 0;
					Position[1]=tonumber(Position[1]) or 0;
					Position[2]=Position[2] or "";
					Position[3]=tonumber(Position[3]) or 0;
					Position[4]=tonumber(Position[4]) or 0;
					Position[5]=tonumber(Position[5]) or 0;
					Position[6]=tonumber(Position[6]) or 0;
					
					bcl = strfind(Position[2],"/");

					if (not bcl and Position[2] > "") then
						Position[2] = "Interface/LoginScreen/"..Position[2];
					end
					if (Position[6] == "") then
						Position[6] = nil;
					end

					BGround[1] = Position[1];
					BGround[2] = Position[2];
					BGround[3] = LoginCam;
					table.insert(ModelList.sceneData, BGround);
				end

				local _,_, ObjTest = strfind(Objs, "|M%[(.-)%]");
				ObjTest = ObjTest or "Vide";
				if(ObjTest ~= "Vide") then
					local lmusic, ltime  = strsplit(",", ObjTest);
					login_music_path = lmusic or "";
					login_music_time_in_seconds = tonumber(ltime) or 121;

					bcl = strfind(login_music_path,"/");

					if (not bcl) then
						login_music_path = "Interface/LoginMusic/"..login_music_path;
					end
				end
				
				local _,_, ObjTest = strfind(Objs, "|P%[(.-)%]");
				ObjTest = ObjTest or "Vide";
				if(ObjTest ~= "Vide") then
					local lmusic, ltime  = strsplit(",", ObjTest);
					LoginGlueAmbience = lmusic or "DARKPORTAL";
					LoginGlueAmbienceVol = tonumber(ltime) or 4.0;
				else
					LoginGlueAmbience = "DARKPORTAL";
					LoginGlueAmbienceVol = 4.0;
				end
				
			end
		else
			login_on = 2;
		end
	else
		PageAlert =  Objs;
	end
	
	ModelList.modelCount = nbrObjet;
	if(nbrObjet > 0)then
					--HidePage();
				Generate_MScene();

				StopAllSFX( 1.0 );
				StopMusic();
				StopGlueMusic();

				PlayMusic(login_music_path);

				login_on = 1;
	end
	
	pligne = pligne + 1;
	ligne=ligne .. "\n ".. pligne .. "  nbrObjet :" .. nbrObjet .. " login_on :" .. login_on .." ";
	
	return PageAlert;
end

-- ***************************************************************
-- ***************************************************************
function randomScene()
	return (time() % ModelList.max_scenes) + 1;
end

function newScene()	-- creates a scene object that gets used internaly
	local s = {parent = CreateFrame("Frame",nil,LoginScene),
				background = ModelList.sceneData[#MScene+1 or 1][2],
				duration = ModelList.sceneData[#MScene+1 or 1][1],
				camera = ModelList.sceneData[#MScene+1 or 1][3]};
	
	s.parent:Hide();
	s.parent:SetSize(LoginScene:GetWidth(), LoginScene:GetHeight());
	s.parent:SetPoint("CENTER");
	s.parent:SetFrameStrata("LOW");

	table.insert(MScene, s);
	return s;
end

function newModel(parent, alpha, light, wSquish, hSquish, objet, camera)	-- creates a new model object that gets used internally but also can be altered after loading
	local mod = CreateFrame("Model",nil,parent);
	
	mod:Hide();
	light = light or {1, 0, 0, -0.707, -0.707, 0.7, 1.0, 1.0, 1.0, 0.8, 1.0, 1.0, 0.8};
	mod:SetModel(objet or "Character/Human/Male/HumanMale.m2");
	mod:SetSize(LoginScene:GetWidth() / wSquish, LoginScene:GetHeight() / hSquish);
	mod:SetPoint("CENTER",0,0);
	mod:SetFrameStrata("LOW");
	mod:SetCamera(camera);
	mod:SetLight(unpack(light));
	mod:SetAlpha(alpha);
	
	return mod;
end

function Generate_MScene()	-- starts the routine for loading all models and scenes
	--pligne = pligne + 1;
	--ligne=ligne .. "\n ".. pligne .. " : Generate_MScene " .. ModelList.modelCount .. " ";
	
	if(ModelList.modelCount >0 ) then
		ModelList.sceneCount = #ModelList.sceneData;

		local counter, i = 0, 0;
		for i=1, ModelList.sceneCount do
			local s = newScene();
			--s.parent:Hide();
			local num, m = num, m;
--ligne = ligne .. " i = "..i.."::\n";
			for num, m in pairs(ModelList) do
				if (type(m)=="table" and num ~= "sceneData") then
					if (m[1] == i) then
				
						table.insert(s, num, newModel(s.parent, m[7], m[8], m[10], m[11], m[14],s.camera));
						counter = counter + 1;
						
						--local lig = m[14] or "vide";
						--ligne = ligne ..  " Num = "..num.." m[14] = "..lig.."\n";
						--end
						ModelList.lastModelNum = num;
					end
				end
			end

		
			if (i == current_scene) then
--ligne=ligne .. "\n s.background "..s.background;
				LoginScreenBackground:Hide();
				if(s.background > "") then
					LoginScreenBackground:SetTexture(s.background);
				end
			end
		end
		
		ModelList.modelCount = counter;
	end
	
end
------- updating and methods
function LoginScreen_OnUpdate(dt)

	--pligne = pligne + 1;
	
	--AccountLogin.UI.Texte:SetText(ligne.." \n AccountLogin_OnShow "..ModelList.modelCount);
	
	if ( LoginScene ~= nil ) then
		--ligne=ligne .. pligne .. " : LoginScreen_OnUpdate1 " .. ModelList.modelCount .. " ";
		
		if(LoginScene:IsShown() and ModelList.modelCount > 0) then
		
			--ligne=ligne .." --> test ";
			
			if (music_timer == login_music_time_in_seconds) then		-- Music timer to loop the background music
				StopMusic()
			elseif (music_timer > login_music_time_in_seconds) then		-- Music timer to loop the background music
				PlayMusic(login_music_path);
				music_timer = 0;
			else
				music_timer = music_timer + dt;
			end
		
			if (blend_start) then				-- Start blend after the loginscreen loaded to hide the setting up frame
				if (blend_start < 0.5) then
					LoginScreenBlend:SetAlpha( 1 - blend_start*2 );
					blend_start = blend_start + dt;
				else
					LoginScreenBlend:SetAlpha(0);
					blend_start = false;
				end
			end

			if (timed_update and timed_update > 5) then		-- frame delayed update to hackfix some errors with blizzard masterrace code
				local num, m = num, m;
			
				for num, m in pairs(ModelList) do
					if (type(m)=="table" and num ~= "sceneData" and m[1] <= ModelList.max_scenes) then
					
						local mod = MScene[m[1]][num];
						
						if(mod ~= nil) then
						
							if (m[15] == "b") then 
								mod:SetFrameStrata("BACKGROUND");
							end
							
							mod:SetModel(m[12]);
							mod:SetPosition(m[4], m[2], m[3]);
							mod:SetFacing(m[5]);
							mod:SetModelScale(m[6]);
							mod:SetSequence(m[9]);
							mod:Show();
						end
					end
				end
			
				blend_start = 0;
				timed_update = false;
			
				MScene[current_scene].parent:Show();
				--Scene_OnStart(current_scene);
			elseif (timed_update) then
				timed_update = timed_update + 1;
			end
		
			local cur = MScene[current_scene];
			if (cur.duration < blend_timer) then		-- Scene and blend timer for next scene and blends between the scenes
		
				if (ModelList.max_scenes > 1) then
					local blend = blend_timer - cur.duration;
					if (blend < ModelList.fade_duration) then
						LoginScreenBlend:SetAlpha( 1 - math.abs( 1 - (blend*2 / ModelList.fade_duration) ) );
					
						if (blend*2 > ModelList.fade_duration and not nextCset) then
							nextC = randomScene();
							if (shuffle_scenes_randomly) then
								if (current_scene == nextC) then
									nextC = ((current_scene+1 > ModelList.max_scenes) and 1) or current_scene + 1;
								end
							else
								nextC = ((current_scene+1 > ModelList.max_scenes) and 1) or current_scene + 1;
							end
							nextCset = true;
						
							local new = MScene[nextC];
							cur.parent:Hide();
							new.parent:Show();
							LoginScreenBackground:Hide();
							if(new.background >"")then
								LoginScreenBackground:SetTexture(new.background);
							end
							Scene_OnEnd(current_scene);
							Scene_OnStart(nextC);
							StopMusic()
							PlayMusic(login_music_path)
						end
					
						blend_timer = blend_timer + dt;
					else
						current_scene = nextC;
						nextCset = false;
						blend_timer = 0;
						LoginScreenBlend:SetAlpha(0);
					end
				else
					blend_timer = 0;
				--Scene_OnEnd(current_scene);
				--Scene_OnStart(current_scene);
				end
		
				---blend_timer = 0;
			else
				blend_timer = blend_timer + dt;
			end
		
			SceneUpdate(dt, current_scene, blend_timer, ModelList.sceneData[current_scene][1]);
		end
	--else
		--ligne=ligne .. "\n ".. pligne .. " : LoginScreen_OnUpdate pas d'update " .. ModelList.modelCount .. " ";
	end
end

function SetScene(sceneID)

	--pligne = pligne + 1;
	--ligne=ligne .. "\n ".. pligne .. " : SetScene " .. sceneID .. " "..ModelList.modelCount .. " ";
	
	if ( ModelList.modelCount > 0 and login_on == 1) then
	
		MScene[current_scene].parent:Hide();
		MScene[sceneID].parent:Hide();
		LoginScreenBackground:Hide();
		
		if(MScene[sceneID].background > "")then
			LoginScreenBackground:SetTexture(MScene[sceneID].background);
		end
		--Scene_OnEnd(current_scene);
		--Scene_OnStart(sceneID);
		current_scene = sceneID;
	end
end

function GetScene(sceneID)
	local curScene = current_scene;
	if (sceneID) then
		if (sceneID <= ModelList.max_scenes and sceneID > 0) then
			curScene = sceneID;
		end
	end
	return curScene, ModelList.sceneData[curScene], GetModel(curScene, true), GetModelData(curScene, true);
end

function GetModelData(refID, allSceneModels)
	local data, count = {}, 0;
	local num, m = num, m;
	
	if (allSceneModels == true) then
		for num, m in pairs(ModelList) do
			if (type(m)=="table" and num ~= "sceneData") then
				if (m[1] == refID) then
					table.insert(data, num, m);
					count = count + 1;
				end
			end
		end
		return (count > 0 and data) or false;
	else
		for num, m in pairs(ModelList) do
			if (type(m)=="table" and num ~= "sceneData") then
				if (m[13] == refID) then
					table.insert(data, num, m);
					count = count + 1;
				end
			end
		end
		return (count > 0 and data) or false;
	end
end

function GetModel(refID, allSceneModels)
	local data, count = {} ,0;
	local num, m = num, m;
	
	if (allSceneModels == true) then
		for num, m in pairs(ModelList) do
			if (type(m)=="table" and num ~= "sceneData") then
				if (m[1] == refID) then
					table.insert(data, num, MScene[m[1]][num]);
					count = count + 1;
				end
			end
		end
		return (count > 0 and data) or false;
	else
		local mData = GetModelData(refID);
		if (mData) then
			for num, m in pairs(mData) do
				table.insert(data, num, MScene[m[1]][num]);
				count = count + 1;
			end
			return (count > 0 and data) or false;
		else
			return false;
		end
	end
end

function HidePage()
	if (ModelList.modelCount > 0) then
		MScene[1].parent:Hide();
	end
	if(LoginScene ~= nil)then
		LoginScene:Hide();
		LoginScreenBackground:Hide();
		LoginScreenBlackBoarderTOP:Hide();
		LoginScreenBlackBoarderBOTTOM:Hide();
		LoginScreenBlend:Hide();
	end
end

function ShowPage(SPage)

	--pligne = pligne + 1;
	--ligne=ligne .. "\n " .. pligne .. " : ShowPage " .. SPage .. " " .. ModelList.modelCount .. " ";

	--if(LoginScene ~= nil)then
		if (ModelList.modelCount > 0) then
			HidePage()
			SetScene(SPage);
			MScene[SPage].parent:Show();
		end
			LoginScene:Show();
			LoginScreenBackground:Show();
			LoginScreenBlackBoarderTOP:Show();
			LoginScreenBlackBoarderBOTTOM:Show();
			LoginScreenBlend:Show();

		--end
	--end
end
------------------------------------------------------------------------------------------------------
------									SCENE SCRIPTING PART									------
------------------------------------------------------------------------------------------------------

-- update function that gets called each frame

function SceneUpdate(dt, sceneID, timer, sceneTime)
	-- Scene scripts that need updates each frame (moving a model for example) go in here.
end

-- on end function that gets called when the scene ends
function Scene_OnEnd(sceneID)
	-- Scene scripts that need an update at the end of a scene (resetting the position of a moving model for example) go in here.
end

-- on start function that gets called when the scene starts
function Scene_OnStart(sceneID)
	-- Scene scripts that need an update at the start of a scene (one time spell visual for example) go in here.
end

-- ***************************************************************
-- ***************************************************************
function InitRandom()
local x, y = GetCursorPosition();
local z = 0;
	x = x*y;
	for y = 0, x do
		z = math.random(1,2);
	end
end
