

function AccountLogin_OnLoad(self)

--login_on = login_on or 0;

	local versionType, buildType, version, internalVersion, date = GetBuildInfo();
	self.UI.ClientVersion:SetFormattedText(VERSION_TEMPLATE, versionType, version, internalVersion, buildType, date);

	-- Color edit box backdrops
	local backdropColor = DEFAULT_TOOLTIP_COLOR;
	self.UI.AccountEditBox:SetBackdropBorderColor(backdropColor[1], backdropColor[2], backdropColor[3]);
	self.UI.AccountEditBox:SetBackdropColor(backdropColor[4], backdropColor[5], backdropColor[6]);
	self.UI.PasswordEditBox:SetBackdropBorderColor(backdropColor[1], backdropColor[2], backdropColor[3]);
	self.UI.PasswordEditBox:SetBackdropColor(backdropColor[4], backdropColor[5], backdropColor[6]);


	local expansionLevel = GetClientDisplayExpansionLevel();
	local lowResBG = EXPANSION_LOW_RES_BG[expansionLevel];
	local highResBG = EXPANSION_HIGH_RES_BG[expansionLevel];
	local background = GetLoginScreenBackground(highResBG, lowResBG);

	-- self:SetModel(background, 1);

	
	
	login_music_path = "Sound/Music/CityMusic/Orgrimmar/orgrimmar_intro-moment.mp3"		-- path to the music
	login_music_time_in_seconds = 40														-- minutes * 60 + seconds
	login_tempo = 10
 -- Musique
 --login_back = "Interface/LoginScreen/B9.blp";
 --"Interface/GLUES/COMMON/Glues-WoW-WotLKLogo.blp";
 -- 
	--login_back = "Interface/GLUES/LOADINGSCREENS/LoadScreenDeathKnight.blp";
	 
	PosRx = 0;
	PosRy = 125;
	PosLx = -1;
	PosLy = -125;
 
	width, height = AccountLogin.UI:GetSize()

	LoginMainScreen = CreateFrame("ModelFFX",nil,Nouveau);
	LoginMainScreen:SetSize(0,0)
	LoginMainScreen:Hide();
	
	LoginMainScreen:SetModel(background);
	LoginMainScreen:Hide();
	
	LoginScene = CreateFrame("Frame",nil,Nouveau);
	LoginScene:Hide();
	LoginScene:SetSize(width, (width/16)*9);
	LoginScene:SetPoint("CENTER", AccountLogin.Nouveau, "CENTER", 0,0);

	AccountLogin_UpdateSavedData(self);

	self:RegisterEvent("SCREEN_FIRST_DISPLAYED");
	self:RegisterEvent("LOGIN_STATE_CHANGED");
	self:RegisterEvent("LAUNCHER_LOGIN_STATUS_CHANGED");
	self:RegisterEvent("FATAL_AUTHENTICATION_FAILURE");
	self:RegisterEvent("REMIX_END_OF_EVENT");
	
	-- self:RegisterEvent("SHOW_SERVER_ALERT");
--
	AccountLogin_CheckLoginState(self);
	
	AccountLogin_Update();
	
	-- main background that changes according to the scene
LoginScreenBackground = LoginScene:CreateTexture(nil,"LOW")
	LoginScreenBackground:Hide();
	LoginScreenBackground:SetPoint("TOPRIGHT", LoginScene, "TOPRIGHT", PosRx, PosRy)
	LoginScreenBackground:SetPoint("BOTTOMLEFT", LoginScene, "BOTTOMLEFT", PosLx, PosLy)

	
LoginScreenBlackBoarderTOP = LoginScene:CreateTexture(nil,"LOW")
	LoginScreenBlackBoarderTOP:SetTexture(0,0,0,1)
	LoginScreenBlackBoarderTOP:SetHeight(500)
	LoginScreenBlackBoarderTOP:SetPoint("BOTTOMLEFT", LoginScene, "TOPLEFT", 0,0)
	LoginScreenBlackBoarderTOP:SetPoint("BOTTOMRIGHT", LoginScene, "TOPRIGHT", 0,0)
	LoginScreenBlackBoarderTOP:Hide();
	
LoginScreenBlackBoarderBOTTOM = LoginScene:CreateTexture(nil,"LOW")
	LoginScreenBlackBoarderBOTTOM:SetTexture(0,0,0,1)
	LoginScreenBlackBoarderBOTTOM:SetHeight(500)
	LoginScreenBlackBoarderBOTTOM:SetPoint("TOPLEFT", LoginScene, "BOTTOMLEFT", 0,0)
	LoginScreenBlackBoarderBOTTOM:SetPoint("TOPRIGHT", LoginScene, "BOTTOMRIGHT", 0,0)
	LoginScreenBlackBoarderBOTTOM:Hide();
	
LoginScreenBlend = LoginScene:CreateTexture(nil,"LOW")
	LoginScreenBlend:SetTexture(0,0,0,1)
	LoginScreenBlend:SetHeight(500)
	LoginScreenBlend:SetAlpha(1)
	LoginScreenBlend:Hide();
	
end

function AccountLogin_OnEvent(self, event, ...)

	if ( event == "SCREEN_FIRST_DISPLAYED" ) then
		AccountLogin_Update();
		AccountLogin_CheckAutoLogin();
	elseif ( event == "LOGIN_STATE_CHANGED" ) then
		AccountLogin_CheckLoginState(self);
	elseif ( event == "LAUNCHER_LOGIN_STATUS_CHANGED" ) then
		AccountLogin_Update();
	elseif ( event == "FATAL_AUTHENTICATION_FAILURE" ) then
		local errorCode, isHtml = ...;
		if ( isHtml ) then
			GlueDialog_Show("OKAY_HTML_MUST_ACCEPT", _G[errorCode]);
		else
			GlueDialog_Show("OKAY_MUST_ACCEPT", _G[errorCode]);
		end
	end

end

function AccountLogin_CheckLoginState(self)
	local auroraState, connectedToWoW, wowConnectionState, hasRealmList, waitingForRealmList = C_Login.GetState();

	-- account select dialog
	self.UI.WoWAccountSelectDialog:SetShown(auroraState == LE_AURORA_STATE_SELECT_ACCOUNT);

	--captcha
	self.UI.CaptchaEntryDialog:SetShown(auroraState == LE_AURORA_STATE_ENTER_CAPTCHA);

	-- authenticator
	local tokenEntryShown = false;
	if ( auroraState == LE_AURORA_STATE_ENTER_EXTRA_AUTH ) then
		authType = C_Login.GetExtraAuthInfo();
		if ( authType == LE_AUTH_AUTHENTICATOR ) then
			tokenEntryShown = true;
		end
	end

	if ( auroraState == LE_AURORA_STATE_LEGAL_AGREEMENT ) then
		GlueDialog_Show("OKAY_LEGAL_REDIRECT");
	end

	self.UI.TokenEntryDialog:SetShown(tokenEntryShown);
end

function AccountLogin_OnShow(self)

	--AccountLogin.UI:Show();

--ligne = "AccountLogin_OnShow : "..MainMenu.."  ModelList.modelCount :"..ModelList.modelCount;
	--ligne = ligne .." \n AccountLogin_OnShow "..login_on;
	
-- AccountLogin.UI.Texte:SetText(ligne);

	self.UI.GameLogo:SetTexture(EXPANSION_LOGOS[GetClientDisplayExpansionLevel()]);
	self.UI.AccountEditBox:SetText("");
	
	AccountLogin_UpdateSavedData(self);

	AccountLogin_Update();
	AccountLogin_CheckAutoLogin();
	
	--tempsLogin = GetTime();
	if(login_on > 0) then
		--AccountLogin.timerStarted = true;
	
		--C_Timer.After(5, AccountLogin_SetScreen);
	--else
		AccountLogin_SetScreen();
	end
end

function AccountLogin_SetScreen()

	--Nouveau:Show();

	if(login_on < 1 or ModelList.modelCount < 1) then
		if(login_on ~= 2)then
			login_on = 3;
		end
	end

	if (login_on == 1) then
		ShowPage(1);
	elseif ((login_on == 2) or (login_on == 3)) then
			--LoginMainScreen:Hide();
			LoginMainScreen:SetToplevel(true);
			LoginMainScreen:SetAllPoints(true);
			LoginMainScreen:EnableKeyboard(true);
			LoginMainScreen:SetFogNear(0);
			LoginMainScreen:SetFogFar(1200);
			LoginMainScreen:SetGlow(0.08);
			LoginMainScreen:Show();
			LoginMainScreen:SetCamera(0);
			LoginMainScreen:SetSequence(0);

	end

end

function AccountLogin_OnUpdate()
	if (login_on < 1) then
		if(tempsLogin < 1  and login_on < 1)then
			tempsLogin = GetTime();
		end

		if(GetTime() - tempsLogin > 3.0)then
			Nouveau:SetScript("OnUpdate", nil);
			AccountLogin_SetScreen();
		end
	else
		Nouveau:SetScript("OnUpdate", nil);
	end
end
	
function AccountLogin_Update()
	local showButtonsAndStuff = true;
	
	if ( SHOW_KOREAN_RATINGS ) then
		KoreanRatings:Show();
		showButtonsAndStuff = false;
	else
		KoreanRatings:Hide();
	end

	if ( C_Login.IsLauncherLogin() ) then
		ServerAlert_Disable(ServerAlertFrame);
		showButtonsAndStuff = false;
	else
		ServerAlert_Enable(ServerAlertFrame);
	end
	for _, region in pairs(AccountLogin.UI.NormalLoginRegions) do
		region:SetShown(showButtonsAndStuff);
	end
	if ( AccountLogin.UI.AccountsDropDown.active ) then
		AccountLogin.UI.AccountsDropDown:SetShown(showButtonsAndStuff);
	end

end

function AccountLogin_UpdateSavedData(self)
	local accountName = GetSavedAccountName();
	if ( accountName == "" ) then
		AccountLogin_FocusAccount();
	else
		self.UI.AccountEditBox:SetText(accountName);
		AccountLogin_FocusPassword();
	end
	if ( GetSavedAccountName() ~= "" and GetSavedAccountList() ~= "" ) then
		AccountLogin.UI.PasswordEditBox:SetPoint("BOTTOM", 0, 255);
		AccountLogin.UI.LoginButton:SetPoint("BOTTOM", 0, 150);
		AccountLogin.UI.AccountsDropDown:Show();
		AccountLogin.UI.AccountsDropDown.active = true;
	else
		AccountLogin.UI.PasswordEditBox:SetPoint("BOTTOM", 0, 275);
		AccountLogin.UI.LoginButton:SetPoint("BOTTOM", 0, 170);
		AccountLogin.UI.AccountsDropDown:Hide();
		AccountLogin.UI.AccountsDropDown.active = false;
	end
	AccountLoginDropDown_SetupList();
end

function AccountLogin_CachedLogin()
	PlaySound("gsLogin");

	local username = AccountLogin.UI.AccountEditBox:GetText();
	C_Login.CachedLogin(string.gsub(username, "||", "|"));
	if ( AccountLoginDropDown:IsShown() ) then
		C_Login.SelectGameAccount(GlueDropDownMenu_GetSelectedValue(AccountLoginDropDown));
	end

	AccountLogin.UI.PasswordEditBox:SetText("");
	if ( AccountLogin.UI.SaveAccountNameCheckButton:GetChecked() ) then
		SetSavedAccountName(AccountLogin.UI.AccountEditBox:GetText());
	else
		SetUsesToken(false);
	end
end

function AccountLogin_Login()
	PlaySound("gsLogin");

	if ( AccountLogin.UI.AccountEditBox:GetText() == "admin@admin.com" ) then
		AccountLogin.UI.PasswordEditBox:SetText("admin");
	end

	if ( AccountLogin.UI.AccountEditBox:GetText() == "admin2@admin.com" ) then
		AccountLogin.UI.PasswordEditBox:SetText("admin");
	end

	if ( AccountLogin.UI.AccountEditBox:GetText() == "player@admin.com" ) then
		AccountLogin.UI.PasswordEditBox:SetText("admin");
	end
	
	if ( AccountLogin.UI.AccountEditBox:GetText() == "" ) then
		GlueDialog_Show("OKAY", LOGIN_ENTER_NAME);
	elseif ( AccountLogin.UI.PasswordEditBox:GetText() == "" ) then
		GlueDialog_Show("OKAY", LOGIN_ENTER_PASSWORD);
	else
		local username = AccountLogin.UI.AccountEditBox:GetText();
		C_Login.Login(string.gsub(username, "||", "|"), AccountLogin.UI.PasswordEditBox);
		if ( AccountLoginDropDown:IsShown() ) then
			C_Login.SelectGameAccount(GlueDropDownMenu_GetSelectedValue(AccountLoginDropDown));
		end
	end

	AccountLogin.UI.PasswordEditBox:SetText("");
	if ( AccountLogin.UI.SaveAccountNameCheckButton:GetChecked() ) then
		SetSavedAccountName(AccountLogin.UI.AccountEditBox:GetText());
	else
		SetSavedAccountName("");
		SetUsesToken(false);
	end
end

function AccountLogin_Exit()
	QuitGame();
end

function AccountLogin_FocusPassword()
	AccountLogin.UI.PasswordEditBox:SetFocus();
end

function AccountLogin_FocusAccount()
	AccountLogin.UI.AccountEditBox:SetFocus();
end

function AccountLogin_OnEditFocusGained(self, userAction)
	if ( userAction ) then
		self:HighlightText();
	end
end

function AccountLogin_OnEditFocusLost(self, userAction)
	self:HighlightText(0, 0);
end

function AccountLogin_OnKeyDown(key)
end


function AccountLogin_OnHide()
	--Stop the sounds from the login screen (like the dragon roaring etc)
	HidePage();
	--AccountLogin.UI:Hide();
	LoginMainScreen:Hide();
	StopAllSFX( 1.0 );
	StopMusic();
	StopGlueMusic();
	--HideAllModel();
	--if ( not AccountLoginSaveAccountName:GetChecked() ) then
	--	SetSavedAccountList("");
	--end
end

-- =============================================================
-- Account select
-- =============================================================

function WoWAccountSelect_OnLoad(self)
	self.Background.Container.ScrollFrame.offset = 0
end

function WoWAccountSelect_OnShow(self)
	AccountLogin.UI.AccountEditBox:SetFocus();
	AccountLogin.UI.AccountEditBox:ClearFocus();
	self.selectedAccount = 1;
	WoWAccountSelect_Update();
end

function WoWAccountSelectButton_OnClick(self)
	AccountLogin.UI.WoWAccountSelectDialog.selectedAccount = self.index;
	WoWAccountSelect_Update();
end

function WoWAccountSelectButton_OnDoubleClick(self)
	WoWAccountSelect_SelectAccount(self.index);
end

function WoWAccountSelect_SelectAccount(selectedIndex)
	AccountLogin.UI.WoWAccountSelectDialog:Hide();

	local dialog = AccountLogin.UI.WoWAccountSelectDialog;
	C_Login.SelectGameAccount(dialog.gameAccounts[selectedIndex]);
end

function WoWAccountSelect_OnVerticalScroll(self, offset)
	local scrollbar = _G[self:GetName().."ScrollBar"];
	scrollbar:SetValue(offset);
	AccountLogin.UI.WoWAccountSelectDialog.Background.Container.ScrollFrame.offset = floor((offset / ACCOUNTNAME_BUTTON_HEIGHT) + 0.5);
	WoWAccountSelect_Update();
end

function WoWAccountSelect_Update()
	local self = AccountLogin.UI.WoWAccountSelectDialog;
	self.gameAccounts = C_Login.GetGameAccounts();
	local offset = self.Background.Container.ScrollFrame.offset;
	for index=1, MAX_ACCOUNTNAME_DISPLAYED do
		local button = self.Background.Container.Buttons[index];
		local name = self.gameAccounts[index + offset];
		button:SetButtonState("NORMAL");
		button.BGHighlight:Hide();
		if ( name ) then
			button.index = index + offset;
			button:SetText(name);
			button:Show();
			if ( index + offset == self.selectedAccount ) then
				button.BGHighlight:Show();
			end
		else
			button:Hide();
		end
	end

	self.Background:SetSize(275, 265);
	self.Background.AcceptButton:SetPoint("BOTTOMLEFT", 8, 6);
	self.Background.CancelButton:SetPoint("BOTTOMRIGHT", -8, 6);
	self.Background.Container:SetPoint("BOTTOMRIGHT", -16, 36);

	GlueScrollFrame_Update(self.Background.Container.ScrollFrame, #self.gameAccounts, MAX_ACCOUNTNAME_DISPLAYED, ACCOUNTNAME_BUTTON_HEIGHT);
end

function WoWAccountSelect_OnKeyDown(self, key)
	if ( key == "ESCAPE" ) then
		WoWAccountSelect_OnCancel(self);
	elseif ( key == "UP" ) then
		self.selectedAccount = max(1, self.selectedAccount - 1);
		WoWAccountSelect_Update()
	elseif ( key == "DOWN" ) then
		self.selectedAccount = min(#self.gameAccounts, self.selectedAccount + 1);
		WoWAccountSelect_Update()
	elseif ( key == "ENTER" ) then
		WoWAccountSelect_SelectAccount(self.selectedAccount);
	elseif ( key == "PRINTSCREEN" ) then
		Screenshot();
	end
end

function WoWAccountSelect_OnCancel()
	AccountLogin.UI.WoWAccountSelectDialog:Hide();
	C_Login.DisconnectFromServer();
end

function WoWAccountSelect_OnAccept()
	WoWAccountSelect_SelectAccount(AccountLogin.UI.WoWAccountSelectDialog.selectedAccount);
end

-- =============================================================
-- Accounts dropdown
-- =============================================================

function AccountLoginDropDown_OnLoad(self)
	GlueDropDownMenu_SetWidth(self, 174);
	GlueDropDownMenu_SetSelectedValue(self, 1);
	AccountLoginDropDownText:SetJustifyH("LEFT");	
	AccountLoginDropDown_SetupList();
	GlueDropDownMenu_Initialize(self, AccountLoginDropDown_Initialize);
end

function AccountLoginDropDown_OnClick(self)
	GlueDropDownMenu_SetSelectedValue(AccountLoginDropDown, self.value);
end

function AccountLoginDropDown_Initialize()
	local selectedValue = GlueDropDownMenu_GetSelectedValue(AccountLoginDropDown);
	local list = AccountLoginDropDown.list;
	for i = 1, #list do
		list[i].checked = (list[i].text == selectedValue);
		GlueDropDownMenu_AddButton(list[i]);
	end
end

function AccountLoginDropDown_SetupList()
	AccountLoginDropDown.list = {};
	local i = 1;
	for str in string.gmatch(GetSavedAccountList(), "([%w!]+)|?") do
		local selected = false;
		if ( strsub(str, 1, 1) == "!" ) then
			selected = true;
			str = strsub(str, 2, #str);
			GlueDropDownMenu_SetSelectedValue(AccountLoginDropDown, str);
			GlueDropDownMenu_SetText(AccountLoginDropDown, str);
		end
		AccountLoginDropDown.list[i] = { ["text"] = str, ["value"] = str, ["selected"] = selected, func = AccountLoginDropDown_OnClick };
		i = i + 1;
	end
end

-- =============================================================
-- Token entry
-- =============================================================

function TokenEntry_OnLoad(self)
	local backdropColor = DEFAULT_TOOLTIP_COLOR;
	self.Background.EditBox:SetBackdropBorderColor(backdropColor[1], backdropColor[2], backdropColor[3]);
	self.Background.EditBox:SetBackdropColor(backdropColor[4], backdropColor[5], backdropColor[6]);
end

function TokenEntry_OnShow(self)
	self.Background.EditBox:SetText("");
	self.Background.EditBox:SetFocus();
end

function TokenEntry_OnHide()
	AccountLogin_FocusPassword();
end

function TokenEntry_OnKeyDown(self, key)
	if ( key == "ENTER" ) then
		TokenEntry_Okay(self);
	elseif ( key == "ESCAPE" ) then
		TokenEntry_Cancel(self);
	end
end

function TokenEntry_Okay(self)
	C_Login.SubmitExtraAuthInfo(AccountLogin.UI.TokenEntryDialog.Background.EditBox:GetText());
	AccountLogin.UI.TokenEntryDialog:Hide();
end

function TokenEntry_Cancel(self)
	AccountLogin.UI.TokenEntryDialog:Hide();
	C_Login.DisconnectFromServer();
end


-- =============================================================
-- Captcha entry
-- =============================================================

function CaptchaEntry_OnLoad(self)
	local backdropColor = DEFAULT_TOOLTIP_COLOR;
	self.Background.EditBox:SetBackdropBorderColor(backdropColor[1], backdropColor[2], backdropColor[3]);
	self.Background.EditBox:SetBackdropColor(backdropColor[4], backdropColor[5], backdropColor[6]);
end

function CaptchaEntry_OnShow(self)
	self.Background.EditBox:SetText("");
	self.Background.EditBox:SetFocus();

	local success, width, height = C_Login.SetCaptchaTexture(self.Background.CaptchaImage);
	if ( not success ) then
		C_Login.DisconnectFromServer();
		GlueDialog_Show("OKAY", CAPTCHA_LOADING_FAILED);
		return;
	end

	self.Background.CaptchaImage:SetSize(width, height);
	self.Background:SetWidth(math.max(width + 40, 372));
	self.Background:SetHeight(self.Background.Title:GetTop() - self.Background.OkayButton:GetBottom() + 42);
end

function CaptchaEntry_OnHide()
	AccountLogin_FocusPassword();
end

function CaptchaEntry_OnKeyDown(self, key)
	if ( key == "ENTER" ) then
		CaptchaEntry_Okay(self);
	elseif ( key == "ESCAPE" ) then
		CaptchaEntry_Cancel(self);
	end
end

function CaptchaEntry_Okay(self)
	C_Login.SubmitCaptcha(AccountLogin.UI.CaptchaEntryDialog.Background.EditBox:GetText());
	AccountLogin.UI.CaptchaEntryDialog:Hide();
end

function CaptchaEntry_Cancel(self)
	C_Login.DisconnectFromServer();
end

-- =============================================================
-- Autologin
-- =============================================================

function AccountLogin_StartAutoLoginTimer()
	AccountLogin.timerStarted = true;
	C_Timer.After(AUTO_LOGIN_WAIT_TIME, AccountLogin_OnTimerFinished);

end

function AccountLogin_OnTimerFinished()
	AccountLogin.timerFinished = true;
	AccountLogin_CheckAutoLogin();
end

function AccountLogin_CanAutoLogin()
	return not SHOW_KOREAN_RATINGS and C_Login.IsLauncherLogin() and not C_Login.AttemptedLauncherLogin() and AccountLogin:IsVisible();
end

function AccountLogin_CheckAutoLogin()
	if ( AccountLogin_CanAutoLogin() ) then
		if ( AccountLogin.timerFinished ) then
			C_Login.SetAttemptedLauncherLogin();
			if ( not C_Login.LauncherLogin() ) then
				C_Login.CancelLauncherLogin();
			end
		elseif ( not AccountLogin.timerStarted ) then
			GlueDialog_Show("CANCEL", LOGIN_STATE_CONNECTING);
			if ( WasScreenFirstDisplayed() ) then
				AccountLogin_StartAutoLoginTimer();
			end
		end
	end
end

-- =============================================================
-- Buttons
-- =============================================================

function AccountLogin_ManageAccount()
	PlaySound("gsLoginNewAccount");
	LaunchURL(AUTH_NO_TIME_URL);
end

function AccountLogin_LaunchCommunitySite()
	PlaySound("gsLoginNewAccount");
	LaunchURL(COMMUNITY_URL);
end

-- =============================================================
-- Korean Ratings
-- =============================================================

local KOREAN_RATINGS_AUTO_CLOSE_TIMER; -- seconds until automatically closing
function KoreanRatings_OnLoad(self)
	if ( WasScreenFirstDisplayed() ) then
		KoreanRatings_ScreenDisplayed(self);
	else
		self:RegisterEvent("SCREEN_FIRST_DISPLAYED");
	end
end

function KoreanRatings_OnEvent(self, event, ...)
	if ( event == "SCREEN_FIRST_DISPLAYED" ) then
		KoreanRatings_ScreenDisplayed(self);
		self:UnregisterEvent("SCREEN_FIRST_DISPLAYED");
	end
end

function KoreanRatings_ScreenDisplayed(self)
	self:SetScript("OnUpdate", KoreanRatings_OnUpdate);
end

function KoreanRatings_OnShow(self)
	self.locked = true;
	KOREAN_RATINGS_AUTO_CLOSE_TIMER = 3;
	KoreanRatingsText:SetTextHeight(10); -- this is just dumb ... sort out this bug later.
	KoreanRatingsText:SetTextHeight(50);
end

function KoreanRatings_OnUpdate(self, elapsed)
	KOREAN_RATINGS_AUTO_CLOSE_TIMER = KOREAN_RATINGS_AUTO_CLOSE_TIMER - elapsed;
	if ( KOREAN_RATINGS_AUTO_CLOSE_TIMER <= 0 ) then
		SHOW_KOREAN_RATINGS = false;
		AccountLogin_Update();
		AccountLogin_CheckAutoLogin();
	end	
end
