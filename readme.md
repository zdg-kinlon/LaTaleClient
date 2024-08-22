## Deployment

### 1. Install Visual Studio 2005

- Download Links
  - For Chinese version: `ed2k://|file|cs_vs_2005_pro_dvd.iso|2733268992|9DA1C378BAC22E66A73C9E20EC78CCFB|/`
  - For English version: `ed2k://|file|en_vs_2005_pro_dvd.iso|2893565952|FBC54207E436783D58CBBC13C1BD1D17|/`

### 2. Install DirectSDK

- Download URLs
  - Chinese: `https://www.microsoft.com/zh-cn/download/details.aspx?id=6812`
  - English: `https://www.microsoft.com/en-us/download/details.aspx?id=6812`

### 3. Configure Environment Variables

1. Launch `Visual Studio 2005`.
2. Click "Tools" in the menu bar, then select "Options". Navigate to "Projects and Solutions" -> "VC++ Directories".
   - Under "Show directories for:" on the right, select "Include files". Add a new environment variable "`$(DXSDK_DIR)\Include`".
   - Next, select "Library files" and add a new environment variable "`$(DXSDK_DIR)\Lib\x86`".
   - If `DXSDK_DIR` is not set by default (usually `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`), you need to configure it in your system's environment variables.

### 4. Open the Solution

Navigate to the `LaTaleClient` directory and open `LaTaleClient.sln` to start the project.

### 5. About SPF Game Assets

Due to GitHub's file size limitations, you'll need to source the Season 1 assets (the non-windowed version) yourself.

### 6. Launch Command

```bash
start LaTaleClient.exe /Run:VLOW /LS:127.0.0.1:10000 /LOCALE:JAPAN /PUBLISHER:GAMEPOT
```

# 部署

## 1. 安装Visual Studio 2005

- 中文：`ed2k://|file|cs_vs_2005_pro_dvd.iso|2733268992|9DA1C378BAC22E66A73C9E20EC78CCFB|/`
- 英文：`ed2k://|file|en_vs_2005_pro_dvd.iso|2893565952|FBC54207E436783D58CBBC13C1BD1D17|/`

## 2. 安装DirectSDK

- 中文：`https://www.microsoft.com/zh-cn/download/details.aspx?id=6812`
- 英文：`https://www.microsoft.com/en-us/download/details.aspx?id=6812`

## 3. 配置环境变量

1. 启动`Visual Studio 2005`

2. 点击菜单栏的“工具 -> 选项”，然后选择“项目和解决方案 -> VC++ 目录”

   在右上角的“显示以下内容的目录”中选择“包含文件”，新建环境变量“`$(DXSDK_DIR)\Include`”

   然后选择“库文件”，新建环境变量“`$(DXSDK_DIR)\Lib\x86`”

   `DXSDK_DIR`默认是`C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`，如果没有需要在系统设置环境变量中配置

## 4. 打开解决方案

进入`LaTaleClient`目录，选择`LaTaleClient.sln`启动项目

## 5. 关于SPF的游戏资源

由于Github对上传文件的容量大小存在限制，需要自行寻找Season 1的资源（不支持窗口化的版本）

## 6. 启动命令

```
start LaTaleClient.exe /Run:VLOW /LS:127.0.0.1:10000 /LOCALE:JAPAN /PUBLISHER:GAMEPOT
```

