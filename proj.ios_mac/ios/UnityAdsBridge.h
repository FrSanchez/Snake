//
//  UnityAdsBridge.h
//  UnityAds2Demo
//
//  Created by Solomon Li on 7/15/16.
//
//

#import <Foundation/Foundation.h>
#import <UnityAds/UnityAds.h>


@interface UnityAdsBridge : UIViewController<UnityAdsDelegate, UADSBannerViewDelegate>

+ (UIViewController* ) viewController;

// This is the Ad Unit or Placement that will display banner ads:
@property (strong) NSString* surfacingId;
// This banner view object will be placed at the top of the screen:
@property (strong, nonatomic) UADSBannerView *topBannerView;
// This banner view object will be placed at the bottom of the screen:
@property (strong, nonatomic) UADSBannerView *bottomBannerView;

@end
