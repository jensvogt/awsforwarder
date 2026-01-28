function Component() {}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {

        var targetExe = installer.value("TargetDir") + "/awsforwarder.exe";

        var startMenuDir =
            installer.value("StartMenuDir") + "/AwsForwarder";

        component.addOperation(
            "CreateShortcut",
            targetExe,
            startMenuDir + "/awsforwarder.lnk",
            "workingDirectory=" + installer.value("TargetDir"),
            "iconPath=" + targetExe,
            "description=AwsForwarder"
        );

        component.addOperation(
            "CreateShortcut",
            installer.value("TargetDir") + "/awsforwarder.exe",
            installer.value("DesktopDir") + "/awsforwarder.lnk",
            "workingDirectory=" + installer.value("TargetDir")
        );
    }
};