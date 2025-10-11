SERVER_ALERT_PTR_URL = "http://127.0.0.1/fr/alert";
SERVER_ALERT_URL = "http://127.0.0.1/fr/alert";

MainMenu = MainMenu or "Vide"; --"Interface/Glues/Models/UI_MainMenu_BurningCrusade/UI_MainMenu_BurningCrusade.m2";
MainScreen = MainScreen or nil;
LoginMainScreen = LoginMainScreen or nil;
MainLogin = false;
tempsLogin = GetTime();

mt_count = 10;

ligne = " ";
pligne = 0;
	
anim = false;
nextC, nextCset, blend_start = nextC, nextCset, blend_start;
timed_update, blend_timer, music_timer = 0, 0, 0;
elapsed = 1;
use_random_starting_scene = false	;													-- boolean: false = always starts with sceneID 1   ||   true = starts with a random sceneID
shuffle_scenes_randomly = false;	

current_scene = current_scene or 1;

MScene = {} or MScene;

ModelList = {
		max_scenes = 1,			-- number of scenes you use to shuffle through
		fade_duration = 1,		-- fade animation duration in seconds (to next scene if more than 1 exists)
		modelCount = 0,
		sceneCount = 0,
		lastModelNum = 0,
		sceneData = {}
	} or ModelList;

-- main frame for displaying and positioning of the whole loginscreen
LoginScene = LoginScene or nil;


LoginCam = 0;
width, height = 0,0;

-- main background that changes according to the scene
LoginScreenBackground = LoginScreenBackground or nil;
LoginScreenBlackBoarderTOP = LoginScreenBlackBoarderTOP or nil;
LoginScreenBlackBoarderBOTTOM = LoginScreenBlackBoarderBOTTOM or nil;
LoginScreenBlend = LoginScreenBlend or nil;
LoginGlueAmbience= LoginGlueAmbience or "DARKPORTAL";
LoginGlueAmbienceVol = LoginGlueAmbienceVol or 4.0;

function ServerAlert_OnLoad(self)
	login_on = login_on or 0;

	self:RegisterEvent("SHOW_SERVER_ALERT");

end

function ServerAlert_OnEvent(self, event, ...)
	if ( event == "SHOW_SERVER_ALERT" ) then
		local text = ...;
		--We need to call SetWidth(0) so that GetWidth() doesn't return 0.
		--Clearly.
		--(Not needed in 7.0, see bug 449258)
		
		local AlertText = "";
		if (text > "" and login_on == 0) then
			
			if (ModelList.modelCount == 0) then

				AlertText = PageLogin(text) or "";
				text = AlertText;
			end
		end
		
		if (ModelList.modelCount > 0 or login_on == 2) then
			if(login_on == 2) then
				LoginMainScreen:SetModel(MainMenu);
			else
				if(MainMenu == "Vide") then
					MainMenu = "Interface\\Glues\\Models\\UI_MainMenu_BurningCrusade\\UI_MainMenu_BurningCrusade.m2";
				end
			end

			AccountLogin_SetScreen();
			
			if(login_on ~= 2)then
				text = "";
			end
		end
		
		if(text > "") then
			self.ScrollFrame:SetWidth(0);
			--We have to resize before calling SetText because SimpleHTML frames won't resize correctly.
			self.ScrollFrame.Text:SetWidth(self.ScrollFrame:GetWidth());
			self.ScrollFrame.Text:SetText(text);
			self.isActive = true;
			if ( not self.disabled ) then
				self:Show();
			end
		end
	end
end

function ServerAlert_Disable(self)
	self:Hide()
	self.disabled = true;
end

function ServerAlert_Enable(self)
	self.disabled = false;
	if ( self.isActive ) then
		self:Show();
	end
end
