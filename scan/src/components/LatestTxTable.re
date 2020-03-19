module Styles = {
  open Css;

  let topicBar =
    style([
      width(`percent(100.)),
      display(`flex),
      flexDirection(`row),
      justifyContent(`spaceBetween),
    ]);
  let seeAll = style([display(`flex), flexDirection(`row), cursor(`pointer)]);
  let cFlex = style([display(`flex), flexDirection(`column)]);
  let amount =
    style([fontSize(`px(20)), lineHeight(`px(24)), color(Colors.gray8), fontWeight(`bold)]);
  let rightArrow = style([width(`px(25)), marginTop(`px(17)), marginLeft(`px(16))]);

  let hScale = 20;
  let fullWidth = style([width(`percent(100.0)), display(`flex)]);
  let blockContainer = style([minWidth(`px(60))]);
  let statusContainer =
    style([
      maxWidth(`px(95)),
      display(`flex),
      flexDirection(`row),
      alignItems(`center),
      justifyContent(`center),
    ]);
  let logo = style([width(`px(20))]);

  let heightByMsgsNum = (numMsgs, mt) =>
    style([
      minHeight(numMsgs <= 1 ? `auto : `px(numMsgs * hScale)),
      marginTop(`px(numMsgs <= 1 ? 0 : mt)),
    ]);
};

[@react.component]
let make = () => {
  let txsOpt = TxHook.latest(~limit=10, ());
  let txs = txsOpt->Belt.Option.mapWithDefault([], ({txs}) => txs);

  <>
    <div className=Styles.topicBar>
      <Text
        value="Latest Transactions"
        size=Text.Xxl
        weight=Text.Bold
        block=true
        color=Colors.gray8
      />
      {switch (txsOpt) {
       | Some(txs) =>
         <div className=Styles.seeAll onClick={_ => Route.redirect(Route.TxHomePage)}>
           <div className=Styles.cFlex>
             <span className=Styles.amount>
               {txs.totalCount |> Format.iPretty |> React.string}
             </span>
             <VSpacing size=Spacing.xs />
             <Text
               value="ALL TRANSACTIONS"
               size=Text.Sm
               color=Colors.bandBlue
               spacing={Text.Em(0.05)}
               weight=Text.Medium
             />
           </div>
           <img src=Images.rightArrow className=Styles.rightArrow />
         </div>
       | None => React.null
       }}
    </div>
    <VSpacing size=Spacing.lg />
    <THead>
      <Row>
        <HSpacing size={`px(12)} />
        <Col size=0.92>
          <div className=Styles.fullWidth>
            <Text
              value="TX HASH"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <Col>
          <div className={Css.merge([Styles.fullWidth, Styles.blockContainer])}>
            <Text
              value="BLOCK"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <Col size=0.5>
          <div className=Styles.statusContainer>
            <Text
              value="STATUS"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <Col size=3.>
          <div className=Styles.fullWidth>
            <Text
              value="ACTIONS"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <HSpacing size={`px(20)} />
      </Row>
    </THead>
    {txs
     ->Belt.List.map(({blockHeight, hash, messages, success}) => {
         let numMsgs = messages->Belt_List.size;
         <TBody key={hash |> Hash.toHex}>
           <Row minHeight={`px(30)}>
             <HSpacing size={`px(12)} />
             <Col size=0.92>
               <div className={Styles.heightByMsgsNum(numMsgs, 0)}>
                 <TxLink txHash=hash width=110 />
               </div>
             </Col>
             <Col>
               <div
                 className={Css.merge([
                   Styles.heightByMsgsNum(numMsgs, -4),
                   Styles.blockContainer,
                 ])}>
                 <TypeID.Block id={ID.Block.ID(blockHeight)} />
               </div>
             </Col>
             <Col size=0.5>
               <div className={Styles.heightByMsgsNum(numMsgs, -8)}>
                 <div className=Styles.statusContainer>
                   <img src={success ? Images.success : Images.fail} className=Styles.logo />
                 </div>
               </div>
             </Col>
             <Col size=3.>
               {messages
                ->Belt_List.toArray
                ->Belt_Array.map(msg =>
                    <>
                      <VSpacing size=Spacing.sm />
                      <Msg msg success width=385 />
                      <VSpacing size=Spacing.sm />
                    </>
                  )
                ->React.array}
             </Col>
             <HSpacing size={`px(20)} />
           </Row>
         </TBody>;
       })
     ->Array.of_list
     ->React.array}
  </>;
};
