//
//  WebKitLogging.m
//  WebKit
//
//  Created by Darin Adler on Sun Sep 08 2002.
//  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
//

#import "WebKitLogging.h"

WebLogChannel WebKitLogTiming =       { 0x00000020, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogLoading =      { 0x00000040, "WebKitLogLevel", WebLogChannelUninitialized };

WebLogChannel WebKitLogFontCache =    { 0x00000100, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogDownload =     { 0x00000800, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogDocumentLoad = { 0x00001000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogPlugins =      { 0x00002000, "WebKitLogLevel", WebLogChannelUninitialized };

WebLogChannel WebKitLogEvents =       { 0x00010000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogView =         { 0x00020000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogRedirect =     { 0x00040000, "WebKitLogLevel", WebLogChannelUninitialized };
