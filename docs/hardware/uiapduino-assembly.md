# UIAPduino筐体設計用Fusionアセンブリ

## 作業結果

2026-10-30：**JoystickMouse_Assembly v2を作成・保存し、閉じて再読み込み後の外部リンク・配置・基準ジオメトリを検証済み。**

筐体の実形状は作成していない。Base/Coverは空コンポーネント。参照元PCBのコピー、リンク解除、形状編集は行っていない。現在のFusionアクティブ文書はJoystickMouse_Assembly v2、保存済み・変更なし。

長さ・座標はすべてmm、回転は度。根拠は[3D解析資料](uiapduino-3d-analysis.md)とFusion MCPの実測結果。実物と3Dモデルの寸法一致は別途検証が必要。

## 保存先と参照方式

| 対象 | 保存名・バージョン | Fusion保存先 | 状態 |
|---|---|---|---|
| 参照元PCB | UIAPduino_PCB_Reference v1 | CoderDojo香椎 / parts module | 外部リンクの参照元、今回の続行で追加保存なし |
| 新規アセンブリ | JoystickMouse_Assembly v2 | CoderDojo香椎 / joystick mouse | 新規別デザイン、保存・再読み込み済み |
| 既存joystick mouse | joystick mouse | CoderDojo香椎 / joystick mouse | 編集・上書きなし |

- PCB DataFile ID：`urn:adsk.wipprod:dm.lineage:HUjWuCkVT2WfYY_vBCW3Vw`
- Assembly DataFile ID：`urn:adsk.wipprod:dm.lineage:YHa2ouJqT2GGmR79FPjphg`
- parts module folder ID：`urn:adsk.wipprod:fs.folder:co.mBcuz2q2TKWt4BPCjsA1yw`
- joystick mouse folder ID：`urn:adsk.wipprod:fs.folder:co.D84NbNdlTBSOea4LGOIf0w`

同名ファイルが存在しないことを確認して新規作成。まず空のアセンブリv1を同一プロジェクト内に保存し、外部リンクと基準を追加してv2を保存した。両ファイルのクラウド処理完了isComplete=trueを確認した。

参照方式は `Occurrences.addByInsert(sourceDataFile, transform, True)` による**外部リンク**。再読み込み後もisReferencedComponent=true。Assembly.childReferencesは上記PCB DataFileのv1を返し、Document.isUpToDate=true。コピーへの代替は行っていない。

## アセンブリ構造

```text
JoystickMouse_Assembly
├── UIAPduino_PCB                      ローカル参照用コンテナ、固定
│   └── UIAPduino_PCB_Reference v1     外部リンク、親に固定
│       ├── ボード
│       ├── 1-copper
│       ├── 16-copper
│       ├── 1-soldermask
│       ├── 16-soldermask
│       └── Packages
│           └── 43要素（FRAME1を含む）
├── Enclosure
│   ├── Base                          ボディ0、スケッチ0
│   └── Cover                         ボディ0、スケッチ0
└── Reference_Geometry                基準面3、スケッチ4、ボディ0
```

参照元の49 occurrence・65 BRepボディを保持。新規構造全体は55 occurrence。
PCB/Electronic_Componentsに相当する元のボード/Packages等は、リンク先を編集せず、元の名前と階層を保持した。
新規デザインは履歴ありのHybrid構成とし、ローカルのBase/Coverを後続の筐体設計で編集できるようにした。

UIAPduino_PCBとReference_GeometryはisGrounded=true。外部リンクoccurrenceはisGroundToParent=true。この固定は新規アセンブリ内の配置設定であり、元PCBの部品配置は変更していない。

## PCB配置・座標系

元PCBの幅方向を+X、長手方向を+Y、実装面側を+Zとして維持。
**基板本体上面をZ=0**にするため、外部リンク全体をZ=-1.50だけ平行移動した。ソルダーマスク上面をZ=0にしたものではない。

```text
p_assembly = T × p_source

T（座標単位mm） =
[1 0 0  0
 0 1 0  0
 0 0 1 -1.50
 0 0 0  1]
```

Fusion APIの内部単位はcmなので、Matrix3D.translationに(0, 0, -0.15)を設定した。回転・拡大縮小なし。個々の電子部品を移動する処理は行っていない。

| 項目 | 元PCB座標 | アセンブリ座標 | 根拠 |
|---|---|---|---|
| 基板本体最小点 | (0, 0, 0) | (0, 0, -1.50) | BRep実測 |
| 基板本体最大点 | (17.78, 33.02, 1.50) | (17.78, 33.02, 0) | BRep実測 |
| 基板幅×長さ×厚み | 17.78×33.02×1.50 | 17.78×33.02×1.50 | 実測、不変 |
| CN1部品原点 | (8.89, 28.3718, 1.5525) | (8.89, 28.3718, 0.0525) | occurrence配置 |
| CN1簡略形状最小点 | (2.72, 25.2369, 1.5525) | (2.72, 25.2369, 0.0525) | BRep実測 |
| CN1簡略形状最大点 | (15.06, 33.1129, 1.6525) | (15.06, 33.1129, 0.1525) | BRep実測 |
| 現モデル最大部品Z（D1） | 3.0255 | 1.5255 | 実測、実物未検証 |
| 現モデル最小Z（下面銅） | -0.07 | -1.57 | 実測 |

USB-Cパッケージ名はUSB-TYPE-C-016、配置回転は+Z軸回り180度。CN1は基板の+Y端側に存在する。**実挿入口の中心高さ・実挿抜方向は未確認**。部品原点を挿入口中心とみなしていない。前回解析の「+Yから-Yへ挿入」は推定にとどまるため、確定した方向軸は作成していない。

## 作成した基準ジオメトリ

すべて新規Reference_Geometry内に配置した。

| 名前 | 内容・実測確認 | 参照性 |
|---|---|---|
| PCB_Top_Z0 | 基板本体上面、Z=0 | 原点XY面からの固定オフセット面 |
| PCB_Bottom_Zminus1_50 | 基板本体下面、Z=-1.50 | 同上 |
| Model_Max_Component_Z1_5255_UNVERIFIED_REAL | 現モデルの最大部品高さ、Z=1.5255 | 固定オフセット面。実物の必要天井高さではない |
| PCB_Outline_Reference | 基板上面外周、52曲線（24キャスタレーションを含む） | 52曲線すべてisReference=true、project2(..., True) |
| PCB_Mounting_Holes | 径1.70の3円 | 3円すべてisReference=true |
| USB_XY_Placement_NOT_Mating_Axis | X=8.89、Y=28.3718～33.02、Z=0の構築線と、CN1原点(8.89,28.3718,0.0525)の点 | 固定座標による補助基準。実挿入軸ではない |
| USB_Unconfirmed_Notes | USB実中心軸・挿抜方向は未確認、モデル高さは実クリアランスではない旨の英語注記 | 注記スケッチ。通常は非表示 |

取付穴参照中心（Z=0）：(1.524,31.496)、(16.256,31.496)、(5.080,1.524)。径はいずれも1.70。
投影参照は元PCBの外部リンク形状に結び付くが、固定オフセット面・USB補助線・点・注記は解析時の値である。**将来PCBを更新した際は、固定値と新モデルを再照合すること。** 外部参照の更新後の動作を試すために元PCBを変更する検証は行っていない。

USBの注意事項とモデル最大高さの意味はReference_GeometryのJoystickMouse属性にも記録。
USB開口・筐体壁・ボス・蓋等の実形状は未作成。

## 保存後・再読み込み後の検証

### 再読み込み

- PCB v1：保存済み・変更なし・クラウド処理完了を確認して文書を閉じ、Documents.openで再度開いた。正常に読み込み、変更なし。
- Assembly v2：保存後のクラウド処理完了を確認。変更なしの文書を閉じてDocuments.openで再度開いた。
- 再読み込み後、AssemblyはisModified=false、isSaved=true、isUpToDate=true。PCB外部リンクを正常に解決。

### 配置・形状・参照

再読み込み後、参照元49 occurrenceと新アセンブリ内の対応49 occurrenceについて比較した。

| 検査 | 結果 |
|---|---|
| 部品階層・対応数 | 49/49対応 |
| 全transform2成分 | 回転不変、Z=-1.50の移動のみ |
| 65 BRepの包絡min/max | 指定平行移動後に一致 |
| 各ボディのvolume・面数・エッジ数 | 一致 |
| 外部リンク | isReferencedComponent=true |
| 参照元DataFile | PCB v1、正しいID |
| 固定状態 | 親コンテナ固定、リンクを親に固定 |
| 基準面Z | 0、-1.50、1.5255 |
| 外形・穴投影 | 52+3曲線すべてリンク参照 |
| Base/Cover | 各ボディ0、スケッチ0 |
| タイムライン | 非Healthy項目0 |
| 元PCB変更状態 | isModified=false、v1維持 |

比較許容差：座標は0.000001 mm、変換行列はAPI内部値で0.0000001、体積は0.00000001 cm³。これはモデル検査の数値許容差であり実物の製造公差ではない。BRep完全同一性の形式的証明や実物検証ではなく、寸法・配置・体積・面/エッジ数の一致確認である。

## Electronicsとの関連付け・元データ保護

最終確認した参照関係：

```text
UIAPduino_Pro_Micro fprj v2
├── UIAPduino_Pro_Micro fsch v1
└── UIAPduino_Pro_Micro fbrd v2
    └── UIAPduino_PCB_Reference f3d v1

JoystickMouse_Assembly v2
└── UIAPduino_PCB_Reference f3d v1（同じDataFile）
```

前回、PCB初回保存のsaveAsに伴いFusionがfprj/fbrdをv1→v2へ連動保存し、作業を停止した。その状態を維持して続行する明示承認を得た。
**今回のアセンブリ作成ではfprj v2、fbrd v2、fsch v1、PCB v1を維持し、追加の元文書保存・形状変更・関連付け切断は実施していない。** 全文書の変更状態はfalseを確認。
初回保存前から存在した2D基板の未保存変更の内容と、旧v1との完全な差分は未確認のまま。

## 発生したエラーと対応

- 子occurrenceへのisGrounded設定は「sub componentでは利用不可」で失敗。直後の読み取りでアセンブリが空の状態であることを確認。公式API説明を確認し、子の固定に対応するisGroundToParentを使って正常作成した。リンク挿入のコピー代替はなし。
- アクティブ文書を前提とした安全確認が失敗したため、保存先DataFile IDから対象文書を取得・明示的にアクティブ化する方式へ変更。元PCBへの書き込みは実行していない。
- 日本語名を条件にした対象検索がStopIterationとなった。列挙して確認したボディを取得し、17.78×33.02の外形と上面Z=0を条件検証してから基準作成を実施した。名前検索が一致しなかった内部原因は未確定。
- 読み取り検証でasArrayのtupleへ代入しTypeErrorとなった。ローカルのlistへ変換して計算し、再検証成功。Fusion形状の変更なし。
- これらの対応後、保存と再読み込みを含む最終検証で不一致0、非Healthy項目0を確認した。

## 未確認・保留項目

- USBの実挿入口中心軸・実挿抜方向・シェル寸法・ケーブル開口寸法。
- 36要素が3Dパッケージ未登録のため、実物の最大高さ・部品寸法とモデルの一致。現モデル最大Z=1.5255を実物の筐体必要高さには使用しない。
- 製品V1.4と設計リビジョンの厳密な一致、実基板公差、はんだ・配線・相手コネクタ空間。
- 将来のPCB更新後の固定基準値再検証。
- 筐体の実形状、組立・接続操作空間、実物との干渉検証は今回の対象外。

## 使用APIと取得方法

- fusion_mcp_read：document一覧、apiDocumentation。
- fusion_mcp_execute：元PCB・関連文書は読み取りのみ。新規アセンブリへ構造と基準を作成し、その文書だけ保存。
- Document.saveAs/save/close、Documents.add/open、Design.designIntent/designType、Occurrence.isGrounded/isGroundToParent。
- Occurrences.addNewComponent/addByInsert、transform2、BRepBody.preciseBoundingBox/volume、BRepFace.geometry/loops、Circle3D、Plane。
- ConstructionPlaneInput.setByOffset、Sketch.project2、SketchEntity.isReference、SketchTexts.createInput3/setAsMultiLine、Attributes。
- DataFile.childReferences/versionNumber/isComplete、Document.isUpToDate/isModified、タイムラインhealthState。
- Fusion内部長さcmを10倍してmm記録。新規PCB形状を作るAPI、リンク解除、コピー、Undo/Redoは使用していない。

