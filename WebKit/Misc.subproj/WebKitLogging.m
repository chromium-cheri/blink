/*
 * Copyright (C) 2005 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "WebKitLogging.h"

WebLogChannel WebKitLogTiming =       		{ 0x00000020, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogLoading =      		{ 0x00000040, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogFontCache =    		{ 0x00000100, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogFontSubstitution =	{ 0x00000200, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogDownload =     		{ 0x00000800, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogDocumentLoad =		{ 0x00001000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogPlugins =		{ 0x00002000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogEvents =       		{ 0x00010000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogView =         		{ 0x00020000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogRedirect =     		{ 0x00040000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogPageCache =              { 0x00080000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogCacheSizes =             { 0x00100000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogFormDelegate = 	        { 0x00200000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogFileDatabaseActivity =   { 0x00400000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogHistory =                { 0x00800000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogBindings =               { 0x01000000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogFontSelection =	        { 0x02000000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogEncoding =               { 0x04000000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogLiveConnect =            { 0x08000000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogBackForward =            { 0x10000000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogProgress =               { 0x20000000, "WebKitLogLevel", WebLogChannelUninitialized };
WebLogChannel WebKitLogPluginEvents =           { 0x40000000, "WebKitLogLevel", WebLogChannelUninitialized };
