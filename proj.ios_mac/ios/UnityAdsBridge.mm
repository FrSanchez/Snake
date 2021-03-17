//
//  UnityAdsBridge.m
//  UnityAds2Demo
//
//  Created by Solomon Li on 7/15/16.
//
//

#import "UnityAdsNativeAPI.h"
#import "UnityAdsBridge.h"
#import "AppController.h"
#import "UnityReward.h"

@implementation UnityAdsBridge

+ (UIViewController* ) viewController {
    
    UIApplication* app = [UIApplication sharedApplication];
    AppController* controller = (AppController*)[app delegate];
    UIViewController* rootController = [controller.window rootViewController];
    return rootController;
}

#pragma mark -
#pragma mark UnityAdsDelegate

- (void)unityAdsReady:(NSString *)placementId {
    NSLog(@"[UnityAds delegate] unityAdsReady with placementId=%@", placementId);
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString *)message{
    NSLog(@"[UnityAds delegate] unityAdsDidError with message=%@ , and error=%ld", message, error);
}

- (void)unityAdsDidStart:(NSString *)placementId{
    NSLog(@"[UnityAds delegate] unityAdsDidStart with placementId=%@", placementId);
}

- (void)unityAdsDidFinish:(NSString *)placementId
          withFinishState:(UnityAdsFinishState)state{
    if(state == kUnityAdsFinishStateCompleted) {
        auto scene = cocos2d::Director::getInstance()->getRunningScene();
        UnityReward* gameScene = dynamic_cast<UnityReward*>(scene);
        if (gameScene) {
            const char *placementIdC = [placementId UTF8String];
            gameScene->rewardPlayer(placementIdC);
        }
    }
}

#pragma mark -
#pragma mark UADSBannerViewDelegate

UnityAdsBridge* s_bridge;

- (void)bannerViewDidLoad:(UADSBannerView *)bannerView {
    // Called when the banner view object finishes loading an ad.
    NSLog(@"Banner loaded for Ad Unit or Placement: %@", bannerView.placementId);
    [bannerView bringSubviewToFront:bannerView];
}

- (void)bannerViewDidClick:(UADSBannerView *)bannerView {
    // Called when the banner is clicked.
    NSLog(@"Banner was clicked for Ad Unit or Placement: %@", bannerView.placementId);
}

- (void)bannerViewDidLeaveApplication:(UADSBannerView *)bannerView {
    // Called when the banner links out of the application.
}

- (void)bannerViewDidError:(UADSBannerView *)bannerView error:(UADSBannerError *)error{
    // Called when an error occurs showing the banner view object.
    NSLog(@"Banner encountered an error for Ad Unit or Placement: %@ with error message %@", bannerView.placementId, [error localizedDescription]);
    // Note that the UADSBannerError can indicate no fill (see API documentation).
}
@end

#pragma mark -
#pragma mark Unity Ads Native API Implementation

void UnityAdsInit (const char *gameIdParameter, bool testMode) {
    
    NSLog(@"[UnityAds] UnityAdsInit");
    
    s_bridge = [UnityAdsBridge new];
    NSString* gameId = [NSString stringWithFormat:@"%s", gameIdParameter];
    [UnityAds initialize:gameId testMode:testMode];
    [UnityAds addDelegate:s_bridge];
}

void UnityAdsShowBanner(const char *parameter) {
    NSString* placementId = [NSString stringWithFormat:@"%s", parameter];
    UADSBannerView* bottomBannerView = [[UADSBannerView alloc] initWithPlacementId:placementId size: CGSizeMake(400, 50)];
    bottomBannerView.delegate = s_bridge;
    bottomBannerView.translatesAutoresizingMaskIntoConstraints = NO;
        [s_bridge.view addSubview:s_bridge.bottomBannerView ];
    [s_bridge.view addConstraints:@[
                                   [NSLayoutConstraint constraintWithItem:bottomBannerView
                                                                attribute:NSLayoutAttributeBottom
                                                                relatedBy:NSLayoutRelationEqual
                                                                   toItem:s_bridge.bottomLayoutGuide
                                                                attribute:NSLayoutAttributeTop
                                                               multiplier:1
                                                                 constant:0],
                                   [NSLayoutConstraint constraintWithItem:bottomBannerView
                                                                attribute:NSLayoutAttributeCenterX
                                                                relatedBy:NSLayoutRelationEqual
                                                                   toItem:s_bridge.view
                                                                attribute:NSLayoutAttributeCenterX
                                                               multiplier:1
                                                                 constant:0]
                                   ]];

    [bottomBannerView load];
    [s_bridge.view bringSubviewToFront:bottomBannerView];
}

bool UnityAdsIsReady (const char *parameter){
    NSString* placementId = [NSString stringWithFormat:@"%s", parameter];
    NSLog(@"[UnityAds] UnityAdsIsReady for placement=%@", placementId);
    return [UnityAds isReady:placementId];
}

void UnityAdsShow (const char *parameter){
    NSString* placementId = [NSString stringWithFormat:@"%s", parameter];
    [UnityAds show:[UnityAdsBridge viewController] placementId:placementId];
}

bool UnityAdsGetDebugMode() {
    NSLog(@"[UnityAds] UnityAdsGetDebugMode");
    return [UnityAds getDebugMode];
}

std::string UnityAdsGetPlacementState(const char* parameter) {
    NSLog(@"[UnityAds] UnityAdsGetPlacementState");
    UnityAdsPlacementState state = [UnityAds getPlacementState];
    switch(state){
        case kUnityAdsPlacementStateReady:
            return "kUnityAdsPlacementStateReady";
        case kUnityAdsPlacementStateNoFill:
            return "kUnityAdsPlacementStateNoFill";
        case kUnityAdsPlacementStateWaiting:
            return "kUnityAdsPlacementStateWaiting";
        case kUnityAdsPlacementStateDisabled:
            return "kUnityAdsPlacementStateDisabled";
        case kUnityAdsPlacementStateNotAvailable:
            return "kUnityAdsPlacementStateNotAvailable";
    }
}

std::string UnityAdsGetVersion() {
    NSLog(@"[UnityAds] UnityAdsGetVersion");
    std::string ret = std::string([[UnityAds getVersion] UTF8String]);
    return ret;
}

bool UnityAdsIsInitialized() {
    NSLog(@"[UnityAds] UnityAdsIsInitialized");
    return [UnityAds isInitialized];
}

bool UnityAdsIsSupported() {
    NSLog(@"[UnityAds] UnityAdsIsSupported");
    return [UnityAds isSupported];
}

void UnityAdsSetDebugMode(bool debugMode) {
    NSLog(@"[UnityAds] UnityAdsSetDebugMode");
    [UnityAds setDebugMode:debugMode];
}

