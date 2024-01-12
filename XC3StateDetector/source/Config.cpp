#include "Config.hpp"

Config::Config(const InitData& init)
	: IScene(init)
{
	if (not getData().ini)
	{
		throw Error{ U"Failed to load `config.ini`" };
	}
	AppLanguage = Parse<String>(getData().ini[U"Language.AppLanguage"]);
	GameLanguage = Parse<String>(getData().ini[U"Language.GameLanguage"]);
	HDMICapture = Parse<String>(getData().ini[U"Device.HDMICapture"]);
	SerialPort = Parse<String>(getData().ini[U"Device.SerialPort"]);
	AccessoryCSVFolderPath = Parse<String>(getData().ini[U"CraftAccessories.AccessoryCSVFolderPath"]);
	AccessoryCSVFilePath = Parse<String>(getData().ini[U"CraftAccessories.AccessoryCSVFilePath"]);
	desireConsecutiveStatus = Parse<bool>(getData().ini[U"CraftAccessories.desireConsecutiveStatus"]);
	enableToastNotification = Parse<bool>(getData().ini[U"CraftAccessories.enableToastNotification"]);
	ScreenshotFolderPath = Parse<String>(getData().ini[U"Screenshot.FolderPath"]);
	ScreenshotFileName = Parse<String>(getData().ini[U"Screenshot.FileName"]);
	ScreenshotDateFormat = Parse<String>(getData().ini[U"Screenshot.DateFormat"]);
	ScreenshotFileFormat = Parse<String>(getData().ini[U"Screenshot.FileFormat"]);

	for (int i = 0; i < LanguageSelection.size(); ++i)
	{
		openableListBoxAppLanguage.emplace_back(Translate[AppLanguage][LanguageSelection[i]]);
		openableListBoxGameLanguage.emplace_back(Translate[AppLanguage][LanguageSelection[i]]);
	}

	size_t AppLanguageIndex = 0;
	for (size_t i = 0; i < LanguageSelection.size(); ++i)
	{
		if (LanguageSelection[i] == getData().AppLanguage)
		{
			AppLanguageIndex = i;
			break;
		}
	}
	openableListBoxAppLanguage.setIndex(AppLanguageIndex);

	size_t GameLanguageIndex = 0;
	for (size_t i = 0; i < LanguageSelection.size(); ++i)
	{
		if (LanguageSelection[i] == getData().GameLanguage)
		{
			GameLanguageIndex = i;
			break;
		}
	}
	openableListBoxGameLanguage.setIndex(GameLanguageIndex);

	for (const auto& info : System::EnumerateWebcams())
	{
		openableListBoxHDMICapture.emplace_back(info.name);
	};
	openableListBoxHDMICapture.setIndexFromItem(HDMICapture);

	for (const auto& info : System::EnumerateSerialPorts())
	{
		openableListBoxSerialPort.emplace_back(U"{} {}"_fmt(info.hardwareID, info.description));
	}
	openableListBoxSerialPort.setIndexFromItem(getData().SerialPort);

	for (int i = 0; i < screenshotDateFormatSelection.size(); ++i)
	{
		openableListBoxScreenshotDateFormat.emplace_back(screenshotDateFormatSelection[i]);
	}
	openableListBoxScreenshotDateFormat.setIndexFromItem(getData().ScreenshotDateFormat);

	for (int i = 0; i < screenshotFileFormatSelection.size(); ++i)
	{
		openableListBoxScreenshotFileFormat.emplace_back(screenshotFileFormatSelection[i]);
	}
	openableListBoxScreenshotFileFormat.setIndexFromItem(getData().ScreenshotFileFormat);

	ScreenshotFileNameTextEditState.text = getData().ScreenshotFileName;
}

void Config::update()
{
	openableListBoxScreenshotFileFormat.update();
	openableListBoxScreenshotDateFormat.update();
	openableListBoxSerialPort.update();
	openableListBoxHDMICapture.update();
	openableListBoxGameLanguage.update();
	openableListBoxAppLanguage.update();
	AppLanguage = LanguageSelection[openableListBoxAppLanguage.getSelectedIndex()];
	GameLanguage = LanguageSelection[openableListBoxGameLanguage.getSelectedIndex()];
	HDMICapture = openableListBoxHDMICapture.getSelectedItem();
	SerialPort = openableListBoxSerialPort.getSelectedItem();
	ScreenshotDateFormat = openableListBoxScreenshotDateFormat.getSelectedItem();
	ScreenshotFileFormat = openableListBoxScreenshotFileFormat.getSelectedItem();
	if (reload)
	{
		changeScene(U"Config");
	}
}

void Config::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	FontAsset(U"TitleFont")(U"Config").draw(titlePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Game Language"]).draw(gameLanguagePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"This Application Language"]).draw(appLanguagePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"HDMI Capture"]).draw(HDMICapturePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Serial Port"]).draw(serialPortPos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Craft Accessories"]).draw(craftAccessoryPos);
	FontAsset(U"TextFont")(AccessoryCSVFolderPath).draw(accessoryCSVFolderPathDrawPos);
	FontAsset(U"TextFont")(AccessoryCSVFilePath).draw(accessoryCSVFilePathDrawPos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Screenshot"]).draw(screenshotPos);
	String now = FormatDateTime(DateTime::Now(), ScreenshotDateFormat);
	FontAsset(U"TextFont")(U"{} : {}{}{}{}"_fmt(Translate[AppLanguage][U"Example"], ScreenshotFolderPath, ScreenshotFileName, now, ScreenshotFileFormat)).draw(screenshotFileExamplePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Notification Enable Toast"]).draw(notificationToastPos);
	SimpleGUI::CheckBox(desireConsecutiveStatus, Translate[AppLanguage][U"Desire all accessories to have the same type of status regardless of special effects (Option)"], consecutiveStatusPos);
	SimpleGUI::CheckBox(enableToastNotification, Translate[AppLanguage][U"Toast notification upon completion"], notificationToastPos);
	openableListBoxScreenshotDateFormat.draw();
	openableListBoxScreenshotFileFormat.draw();

	if (SimpleGUI::Button(Translate[AppLanguage][U"Change the save JSON folder"], accessoryCSVFolderPathPos))
	{
		const auto result = Dialog::SelectFolder(AccessoryCSVFolderPath);
		if (result)
		{
			AccessoryCSVFolderPath = result.value();
		}
	}

	if (SimpleGUI::Button(Translate[AppLanguage][U"If you have a JSON you want to load, set it up here"], accessoryCSVFilePathPos))
	{
		const auto result = Dialog::OpenFile({ FileFilter::CSV() }, AccessoryCSVFilePath);
		if (result)
		{
			AccessoryCSVFilePath = result.value();
		}
	}

	if (SimpleGUI::Button(Translate[AppLanguage][U"Change the save Screenshot folder"], screenshotFolderPos))
	{
		const auto result = Dialog::SelectFolder(getData().ScreenshotFolderPath);
		if (result)
		{
			ScreenshotFolderPath = result.value();
		}
	}

	if (SimpleGUI::TextBox(ScreenshotFileNameTextEditState, screenshotFileNamePos, screenshotFileNameTextWidth, screenshotFileNameMaxCharacters))
	{
		ScreenshotFileName = ScreenshotFileNameTextEditState.text;
	}

	openableListBoxSerialPort.draw();
	openableListBoxHDMICapture.draw();
	openableListBoxGameLanguage.draw();
	openableListBoxAppLanguage.draw();

	if (SimpleGUI::Button(Translate[AppLanguage][U"Apply"], applyButtonPos))
	{
		getData().AppLanguage = AppLanguage;
		getData().GameLanguage = GameLanguage;
		getData().HDMICapture = HDMICapture;
		getData().SerialPort = SerialPort;
		getData().AccessoryCSVFolderPath = AccessoryCSVFolderPath;
		getData().AccessoryCSVFilePath = AccessoryCSVFilePath;
		getData().desireConsecutiveStatus = desireConsecutiveStatus;
		getData().enableToastNotification = enableToastNotification;
		getData().ScreenshotFolderPath = ScreenshotFolderPath;
		getData().ScreenshotFileName = ScreenshotFileName;
		getData().ScreenshotDateFormat = ScreenshotDateFormat;
		getData().ScreenshotFileFormat = ScreenshotFileFormat;
		getData().enableToastNotification = enableToastNotification;

		getData().ini[U"Language.AppLanguage"] = AppLanguage;
		getData().ini[U"Language.GameLanguage"] = GameLanguage;
		getData().ini[U"Device.HDMICapture"] = HDMICapture;
		getData().ini[U"Device.SerialPort"] = SerialPort;
		getData().ini[U"CraftAccessories.AccessoryCSVFolderPath"] = AccessoryCSVFolderPath;
		getData().ini[U"CraftAccessories.AccessoryCSVFilePath"] = AccessoryCSVFilePath;
		getData().ini[U"CraftAccessories.desireConsecutiveStatus"] = desireConsecutiveStatus;
		getData().ini[U"CraftAccessories.enableToastNotification"] = enableToastNotification;
		getData().ini[U"Screenshot.FolderPath"] = ScreenshotFolderPath;
		getData().ini[U"Screenshot.FileName"] = ScreenshotFileName;
		getData().ini[U"Screenshot.DateFormat"] = ScreenshotDateFormat;
		getData().ini[U"Screenshot.FileFormat"] = ScreenshotFileFormat;
		getData().ini.save(U"config.ini");
		reload = true;
	}
}
