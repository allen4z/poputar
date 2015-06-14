//
//  BlueToothHelper.h
//  Guitars
//
//  Created by allen on 15-3-16.
//
//

#import <UIKit/UIKit.h>

#include <CoreBluetooth/CoreBluetooth.h>
#import <CoreBluetooth/CBService.h>

@interface BlueToothHelper : NSObject<CBCentralManagerDelegate,CBPeripheralDelegate>

@property (strong,nonatomic) CBCentralManager *cbCentralMgr;
@property (strong,nonatomic) NSMutableArray *peripheralArray;
@property (strong,nonatomic) NSMutableArray *PeripheralIdentifiers;

+(BlueToothHelper *)shareManager;

-(void)didLoad;

-(void) disconnect;

-(void) scanPeripherals;

-(void) connectPeripherals:(NSString*) uuid;

-(void) sendData:(Byte[]) dataInfo;

-(bool) isConnected;

-(void) addLog:(NSString*)log;

@end
